/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "generatorsetjava.h"
#include "reporthandler.h"

#include "javagenerator.h"
#include "cppheadergenerator.h"
#include "cppimplgenerator.h"
#include "metainfogenerator.h"
#include "classlistgenerator.h"
#include "qdocgenerator.h"
#include "uiconverter.h"
#include "jumptable.h"

#include <QFileInfo>
#include <QtConcurrent>

GeneratorSet *GeneratorSet::getInstance() {
    return new GeneratorSetJava();
}

void dumpMetaJavaTree(const AbstractMetaClassList &classes);

GeneratorSetJava::GeneratorSetJava() :
        no_java(false),
        no_cpp_h(false),
        no_cpp_impl(false),
        no_metainfo(false),
        build_class_list(false),
        build_qdoc_japi(false),
        docs_enabled(false),
        do_ui_convert(false),
        native_jump_table(false),
        target_JDK_version(7),
        doc_dir("../../main/doc/jdoc") {}

QString GeneratorSetJava::usage() {
    QString usage =
        "  --no-java                                 \n"
        "  --no-metainfo                             \n"
        "  --no-cpp-h                                \n"
        "  --no-cpp-impl                             \n"
        "  --convert-to-jui=[.ui-file name]          \n"
        "  --custom-widgets=[file names]             \n"
        "  --target-java-version=8                   \n";

    return usage;
}

bool GeneratorSetJava::readParameters(const QMap<QString, QString> args) {
    no_java = args.contains("no-java");
    no_cpp_h = args.contains("no-cpp-h");
    no_cpp_impl = args.contains("no-cpp-impl");
    no_metainfo = args.contains("no-metainfo");
    build_class_list = args.contains("build-class-list");
    native_jump_table = args.contains("native-jump-table");
    qtjambi_debug_tools = args.contains("qtjambi-debug-tools");

    if (args.contains("build-qdoc-japi")) {
        no_java = true;
        no_cpp_h = true;
        no_cpp_impl = true;
        no_metainfo = true;
        build_qdoc_japi = true;
    }

    if (args.contains("target-java-version")) {
        bool ok = false;
        target_JDK_version = args["target-java-version"].toUInt(&ok);
        if(!ok){
            target_JDK_version = 7;
        }
    }

    if (args.contains("jdoc-dir")) {
        doc_dir =  args.value("jdoc-dir");
    }

    docs_enabled = args.contains("jdoc-enabled");

    if (args.contains("custom-widgets"))
        custom_widgets = args.value("custom-widgets");

    if (args.contains("convert-to-jui")) {
        ui_file_name = args.value("convert-to-jui");
        do_ui_convert = true;

        if (!QFileInfo(ui_file_name).exists()) {
            printf(".ui file '%s' does not exist\n", qPrintable(ui_file_name));
            return false;
        }
    }
    return GeneratorSet::readParameters(args);
}

void GeneratorSetJava::buildModel(const QMap<QString, QString>& features, const QString pp_file) {
    builder.setQtVersion(qtVersionMajor, qtVersionMinor, qtVersionPatch, qtjambiVersionPatch);
    builder.setFileName(pp_file);
    if (!outDir.isNull())
        builder.setOutputDirectory(outDir);
    builder.setFeatures(features);
    builder.build();
    const DocModel* docModel = m_docModelFuture.result();
    m_docModelFuture = {};
    if(docModel){
        builder.applyDocs(docModel);
        if(!docModel->url().isEmpty()){
            this->docsUrl = docModel->url();
            if(!this->docsUrl.endsWith("/"))
                this->docsUrl += "/";
        }
        delete docModel;
    }
}

void GeneratorSetJava::dumpObjectTree() {
    dumpMetaJavaTree(builder.classes());
}

void GeneratorSetJava::generate() {

    // Ui conversion...
    if (do_ui_convert) {
        UiConverter converter;
        converter.setClasses(builder.classes());
        converter.convertToJui(ui_file_name, custom_widgets);
        return;
    } else if (!custom_widgets.isEmpty()) {
        fprintf(stderr, "NOTE: The --custom-widgets option only has an effect when used with --convert-to-jui");
    }

    // Code generation
    QList<Generator *> generators;
    PriGenerator *priGenerator = new PriGenerator;
    JavaGenerator *java_generator = nullptr;
    CppHeaderGenerator *cpp_header_generator = nullptr;
    CppImplGenerator *cpp_impl_generator = nullptr;
    MetaInfoGenerator *metainfo = nullptr;
    JumpTablePreprocessor *jumpTablePreprocessor = nullptr;
    JumpTableGenerator *jumpTableGenerator = nullptr;

    QStringList contexts;
    if (build_qdoc_japi) {
        generators << new QDocGenerator;
        contexts << "QDocGenerator";
    }

    if (native_jump_table) {
        jumpTablePreprocessor = new JumpTablePreprocessor();
        generators << jumpTablePreprocessor;
        contexts << "JumpTablePreprocessor";
    }

    if (!no_java) {
        java_generator = new JavaGenerator;
        java_generator->setTypeSystemByPackage(builder.typeSystemByPackage());
        java_generator->setDocumentationDirectory(doc_dir);
        java_generator->setDocumentationEnabled(docs_enabled);
        java_generator->setNativeJumpTable(native_jump_table);
        java_generator->setTargetJDKVersion(target_JDK_version);
        if (!javaOutDir.isNull())
            java_generator->setJavaOutputDirectory(javaOutDir);
        if (!outDir.isNull())
            java_generator->setLogOutputDirectory(outDir);
        java_generator->setDocsUrl(docsUrl);
        generators << java_generator;

        contexts << "JavaGenerator";
    }

    if (!no_cpp_h) {
        cpp_header_generator = new CppHeaderGenerator(priGenerator);
        if (!cppOutDir.isNull())
            cpp_header_generator->setCppOutputDirectory(cppOutDir);
        generators << cpp_header_generator;
        contexts << "CppHeaderGenerator";
    }

    if (!no_cpp_impl) {
        cpp_impl_generator = new CppImplGenerator(priGenerator);
        cpp_impl_generator->setContainerBaseClasses(builder.containerBaseClasses());
        cpp_impl_generator->setNativeJumpTable(native_jump_table);
        cpp_impl_generator->setQtJambiDebugTools(qtjambi_debug_tools);
        if (!cppOutDir.isNull())
            cpp_impl_generator->setCppOutputDirectory(cppOutDir);
        generators << cpp_impl_generator;
        contexts << "CppImplGenerator";
    }

    if (native_jump_table) {
        jumpTableGenerator = new JumpTableGenerator(jumpTablePreprocessor, priGenerator);
        generators << jumpTableGenerator;
        contexts << "JumpTableGenerator";
    }

    if (build_class_list) {
        generators << new ClassListGenerator;
        contexts << "ClassListGenerator";
    }

    if (!no_metainfo) {
        metainfo = new MetaInfoGenerator(priGenerator);
        metainfo->setQtJambiDebugTools(qtjambi_debug_tools);
        metainfo->setStaticLibraries(m_staticLibraries);
        if (!cppOutDir.isNull())
            metainfo->setCppOutputDirectory(cppOutDir);
        if (!javaOutDir.isNull())
            metainfo->setJavaOutputDirectory(javaOutDir);
        generators << metainfo;
        contexts << "MetaInfoGenerator";
    }

    if (!cppOutDir.isNull())
        priGenerator->setCppOutputDirectory(cppOutDir);

    QList<QFuture<void>> generated;
    for (int i = 0; i < generators.size(); ++i) {
        Generator *generator = generators.at(i);
        generator->setQtVersion(qtVersionMajor, qtVersionMinor, qtVersionPatch, qtjambiVersionPatch);

        if (generator->outputDirectory().isNull())
            generator->setOutputDirectory(outDir);
        generator->setClasses(builder.classes());
        if (printStdout){
            ReportHandler::setContext(contexts.at(i));
            generator->printClasses();
        }else{
            generated << QtConcurrent::run([](Generator *generator, const QString& context){
                         ReportHandler::setContext(context);
                         generator->generate();
            }, generator, contexts.at(i));
        }
    }
    while(!generated.isEmpty()){
        generated.takeFirst().waitForFinished();
    }

    if (metainfo && java_generator) {
        metainfo->writeLibraryInitializers();
    }

    ReportHandler::setContext("PriGenerator");
    priGenerator->setQtVersion(qtVersionMajor, qtVersionMinor, qtVersionPatch, qtjambiVersionPatch);
    if (priGenerator->outputDirectory().isNull())
        priGenerator->setOutputDirectory(outDir);
    priGenerator->setClasses(builder.classes());
    if (printStdout){
        priGenerator->printClasses();
    }else{
        priGenerator->generate();
    }

    QString res;
    res = QString("Classes in typesystem: %1\n"
                  "Generated:\n"
                  "  - java......: %2 (%3)\n"
                  "  - cpp-impl..: %4 (%5)\n"
                  "  - cpp-h.....: %6 (%7)\n"
                  "  - meta-info.: %8 (%9)\n"
                  "  - pri.......: %10 (%11)\n"
                 )
          .arg(builder.classes().size())
          .arg(java_generator ? java_generator->numGenerated() : 0)
          .arg(java_generator ? java_generator->numGeneratedAndWritten() : 0)
          .arg(cpp_impl_generator ? cpp_impl_generator->numGenerated() : 0)
          .arg(cpp_impl_generator ? cpp_impl_generator->numGeneratedAndWritten() : 0)
          .arg(cpp_header_generator ? cpp_header_generator->numGenerated() : 0)
          .arg(cpp_header_generator ? cpp_header_generator->numGeneratedAndWritten() : 0)
          .arg(metainfo ? metainfo->numGenerated() : 0)
          .arg(metainfo ? metainfo->numGeneratedAndWritten() : 0)
          .arg(priGenerator->numGenerated())
          .arg(priGenerator->numGeneratedAndWritten());
    printf("%s\n", qPrintable(res));
    printf("Done, %d warnings (%d known issues)\n", int(ReportHandler::reportedWarnings().size()),
           ReportHandler::suppressedCount());

    QString fileName("reported_warnings.log");
    QFile file(fileName);
    if (!outDir.isNull())
        file.setFileName(QDir(outDir).absoluteFilePath(fileName));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        {
            QTextStream s(&file);
            for(const QString& w : ReportHandler::reportedWarnings()){
                if(!w.contains("Suppressed warning with no text specified"))
                    s << w << Qt::endl;
            }
        }
        file.close();
    }
}

void dumpMetaJavaAttributes(const AbstractMetaAttributes *attr) {
    if (attr->isNative()) printf(" native");
    if (attr->isAbstract()) printf(" abstract");
    if (attr->isFinalInTargetLang()) printf(" final(java)");
    if (attr->isFinalInCpp()) printf(" final(cpp)");
    if (attr->isStatic()) printf(" static");
    if (attr->isPrivate()) printf(" private");
    if (attr->isProtected()) printf(" protected");
    if (attr->isPublic()) printf(" public");
    if (attr->isFriendly()) printf(" friendly");
}

void dumpMetaJavaType(const AbstractMetaType *type) {
    if (!type) {
        printf("[void]");
    } else {
        printf("[type: %s", qPrintable(type->typeEntry()->qualifiedCppName()));
        if (type->getReferenceType()==AbstractMetaType::Reference) printf(" &");
        if (type->getReferenceType()==AbstractMetaType::RReference) printf(" &&");
        for(int i=0; i<type->indirections().size(); i++){
            if(type->indirections()[i]){
                printf("*const ");
            }else{
                printf(" *");
            }
        }

        printf(", %s", qPrintable(type->typeEntry()->qualifiedTargetLangName()));

        if (type->isPrimitive()) printf(" primitive");
        if (type->isEnum()) printf(" enum");
        if (type->isQObject()) printf(" q_obj");
        if (type->isNativePointer()) printf(" n_ptr");
        if (type->isTargetLangString()) printf(" java_string");
        if (type->isTargetLangStringRef()) printf(" java_string");
        if (type->isConstant()) printf(" const");
        if (type->isVolatile()) printf(" volatile");
        printf("]");
    }
}

void dumpMetaJavaArgument(const AbstractMetaArgument *arg) {
    printf("        ");
    dumpMetaJavaType(arg->type());
    printf(" %s", qPrintable(arg->argumentName()));
    if (!arg->defaultValueExpression().isEmpty())
        printf(" = %s", qPrintable(arg->defaultValueExpression()));
    printf("\n");
}

void dumpMetaJavaFunction(const AbstractMetaFunction *func) {
    printf("    %s() - ", qPrintable(func->name()));
    dumpMetaJavaType(func->type());
    dumpMetaJavaAttributes(func);

    // Extra attributes...
    if (func->isSignal()) printf(" signal");
    if (func->isSlot()) printf(" slot");
    if (func->isConstant()) printf(" const");

    printf("\n      arguments:\n");
    for(AbstractMetaArgument *arg : func->arguments())
        dumpMetaJavaArgument(arg);
}

void dumpMetaJavaClass(const AbstractMetaClass *cls) {
    printf("\nclass: %s, package: %s\n", qPrintable(cls->name().replace("$", ".")), qPrintable(cls->package()));
    if (cls->hasVirtualFunctions())
        printf("    shell based\n");
    printf("  baseclass: %s %s\n", qPrintable(cls->baseClassName()), cls->isQObject() ? "'QObject-type'" : "'not a QObject-type'");
    printf("  interfaces:");
    for(AbstractMetaClass *iface : cls->interfaces())
        printf(" %s", qPrintable(iface->name().replace("$", ".")));
    printf("\n");
    printf("  attributes:");
    dumpMetaJavaAttributes(cls);

    printf("\n  functions:\n");
    for(const AbstractMetaFunction *func : cls->functions())
        dumpMetaJavaFunction(func);
}

void dumpMetaJavaTree(const AbstractMetaClassList &classes) {
    for(AbstractMetaClass *cls : classes) {
        dumpMetaJavaClass(cls);
    }
}

