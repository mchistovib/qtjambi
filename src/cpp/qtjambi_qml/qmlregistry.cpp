/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtCore/QtGlobal>
#include <qtjambi/qtjambi_global.h>
#include <QtQml>

#include <QtQml/qqmlprivate.h>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_cast.h>
#include <qtjambi/qtjambi_qml.h>
#include <qtjambi/qtjambi_registry.h>
#include <qtjambi/qtjambi_functionpointer.h>
#include <qtjambi/qtjambi_jobjectwrapper.h>
#include <qtjambi/qtjambi_application.h>
#include "qtjambi_qml_repository.h"
#include "qmlcreateparentfunction.h"
#include "qmlattachedpropertiesfunction.h"
#include "qmlcreatorfunction.h"

class QQmlParserStatus;
class QQmlPropertyValueSource;
class QQmlPropertyValueInterceptor;

const QMetaObject * attachedPropertiesMetaObject(JNIEnv * env, jobject method){
    if(!method)
        return nullptr;
    jclass returnType = jclass(Java::Runtime::Method::getReturnType(env, method));
    const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, returnType);
    if(!meta_object){
        jclass closestClass = resolveClosestQtSuperclass(env, returnType);
        if(closestClass){
            const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
            meta_object = qtjambi_metaobject_for_class(env, returnType, original_meta_object);
        }
    }
    return meta_object;
}

int parserStatusCast(JNIEnv * env, jclass clazz){
    return qtjambi_interface_offset(env, clazz, typeid(QQmlParserStatus));
}

int valueSourceCast(JNIEnv * env, jclass clazz){
    return qtjambi_interface_offset(env, clazz, typeid(QQmlPropertyValueSource));
}

int valueInterceptorCast(JNIEnv *, jclass){
    return -1;
}

int registerQmlListType(const QString& javaName){
    QByteArray listName = "QQmlListProperty<" + javaName.toLatin1().replace(".", "::").replace("/", "::") + '>';
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    return QMetaType::registerNormalizedType(listName,
                                      QtMetaTypePrivate::QMetaTypeFunctionHelper<QQmlListProperty<QObject> >::Destruct,
                                      [](void * ptr, const void * copy)->void*{
                                            const QQmlListProperty<QObject>* _copy = reinterpret_cast<const QQmlListProperty<QObject>*>(copy);
                                            QQmlListProperty<QObject>* list = new(ptr) QQmlListProperty<QObject>();
                                            if(_copy){
                                                list->append = _copy->append;
                                                list->at = _copy->at;
                                                list->clear = _copy->clear;
                                                list->count = _copy->count;
                                                list->data = _copy->data;
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
                                                list->dummy1 = _copy->dummy1;
                                                list->dummy2 = _copy->dummy2;
#else
                                                list->replace = _copy->replace;
                                                list->removeLast = _copy->removeLast;
#endif
                                                list->object = _copy->object;
                                            }
                                            return list;
                                      },
                                      sizeof(QQmlListProperty<QObject>),
                                      QMetaType::MovableType | QMetaType::NeedsConstruction | QMetaType::NeedsDestruction,
                                      nullptr
                                      );
#else
    return registerQmlMetaType({},
                               listName,
                                /*.defaultCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QQmlListProperty<QObject>>::defaultCtr,
                                /*.copyCtr=*/ [](const QtPrivate::QMetaTypeInterface *, void * ptr, const void * copy){
                                                  const QQmlListProperty<QObject>* _copy = reinterpret_cast<const QQmlListProperty<QObject>*>(copy);
                                                  QQmlListProperty<QObject>* list = new(ptr) QQmlListProperty<QObject>();
                                                  if(_copy){
                                                      list->append = _copy->append;
                                                      list->at = _copy->at;
                                                      list->clear = _copy->clear;
                                                      list->count = _copy->count;
                                                      list->data = _copy->data;
                                                      list->replace = _copy->replace;
                                                      list->removeLast = _copy->removeLast;
                                                      list->object = _copy->object;
                                                  }
                                            },
                                /*.moveCtr=*/ [](const QtPrivate::QMetaTypeInterface *, void * ptr, void * copy){
                                                  const QQmlListProperty<QObject>* _copy = reinterpret_cast<const QQmlListProperty<QObject>*>(copy);
                                                  QQmlListProperty<QObject>* list = new(ptr) QQmlListProperty<QObject>();
                                                  if(_copy){
                                                      list->append = std::move(_copy->append);
                                                      list->at = std::move(_copy->at);
                                                      list->clear = std::move(_copy->clear);
                                                      list->count = std::move(_copy->count);
                                                      list->data = std::move(_copy->data);
                                                      list->replace = std::move(_copy->replace);
                                                      list->removeLast = std::move(_copy->removeLast);
                                                      list->object = std::move(_copy->object);
                                                  }
                                            },
                              /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QQmlListProperty<QObject>>::dtor,
                              /*.equals=*/ QtPrivate::QEqualityOperatorForType<QQmlListProperty<QObject>>::equals,
                              /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<QQmlListProperty<QObject>>::lessThan,
                              /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<QQmlListProperty<QObject>>::debugStream,
                              /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<QQmlListProperty<QObject>>::dataStreamOut,
                              /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<QQmlListProperty<QObject>>::dataStreamIn,
                              /*.legacyRegisterOp=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QQmlListProperty<QObject>>::legacyRegisterOp,
                              /*.size=*/ sizeof(QQmlListProperty<QObject>),
                              /*.alignment=*/ alignof(QQmlListProperty<QObject>),
                              /*.typeId=*/ QMetaType::UnknownType,
                              /*.flags=*/ QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<QQmlListProperty<QObject>>::Flags),
                              nullptr,
                              nullptr);
#endif
}

/**
 * this method is used by the qml wrapper
 */
int registerQmlMetaType(const QString& javaName, const QMetaObject *meta_object)
{
    QByteArray _javaName = javaName.toLatin1().replace(".", "::")+"*";

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    int definedType = QMetaType::type(_javaName);
    if(definedType!=QMetaType::UnknownType){
        return definedType;
    }
    int typeId = QMetaType::registerNormalizedType(_javaName,
                                     QtMetaTypePrivate::QMetaTypeFunctionHelper<QObject*>::Destruct,
                                     QtMetaTypePrivate::QMetaTypeFunctionHelper<QObject*>::Construct,
                                     sizeof(QObject*),
                                     QMetaType::TypeFlags( QMetaType::MovableType | QMetaType::PointerToQObject ),
                                     meta_object);
    registerJavaClassForCustomMetaType(typeId, QString(javaName).replace(".", "/").toLatin1());
    return typeId;
#else
    QMetaType definedType = QMetaType::fromName(_javaName);
    if(definedType.id()!=QMetaType::UnknownType){
        return definedType.id();
    }
    return registerQmlMetaType(javaName.toLatin1().replace(".", "/"), _javaName,
                                        /*.defaultCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QObject*>::defaultCtr,
                                        /*.copyCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QObject*>::copyCtr,
                                        /*.moveCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QObject*>::moveCtr,
                                        /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QObject*>::dtor,
                                        /*.equals=*/ QtPrivate::QEqualityOperatorForType<QObject*>::equals,
                                        /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<QObject*>::lessThan,
                                        /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<QObject*>::debugStream,
                                        /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<QObject*>::dataStreamOut,
                                        /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<QObject*>::dataStreamIn,
                                        /*.legacyRegisterOp=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QObject*>::legacyRegisterOp,
                                        /*.size=*/ sizeof(QObject*),
                                        /*.alignment=*/ alignof(QObject*),
                                        /*.typeId=*/ QMetaType::UnknownType,
                                        /*.flags=*/ QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<QObject*>::Flags),
                                        meta_object,
                                        nullptr);
#endif
}

jmethodID findConstructor(JNIEnv * env, jclass clazz, const QString& javaName){
    jmethodID constructor(nullptr);
    QTJAMBI_TRY_ANY{
        constructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject$QDeclarativeConstructor;)V", clazz);
    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
    jmethodID superClassConstructor = nullptr;
    jclass generatedSuperclass = qtjambi_find_generated_superclass(env, clazz);
    if(generatedSuperclass){
        QTJAMBI_TRY_ANY{
            superClassConstructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject$QDeclarativeConstructor;)V", clazz);
        }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
    }else{
        JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class %1 cannot be registered as Qml type since it does not inherit a Qt class.").arg(QString(javaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
    }
    if(!constructor){
        if(superClassConstructor){
            JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class must offer the constructor %1(QDeclarativeConstructor) to register as Qml type.").arg(QString(javaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
        }else{
            QString superClassName = qtjambi_class_name(env, generatedSuperclass).replace(QLatin1Char('$'), QLatin1Char('.'));
            JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class %1 cannot be registered as Qml type because its super class %2 is excluded.").arg(QString(javaName).replace(QLatin1Char('$'), QLatin1Char('.'))).arg(superClassName)) QTJAMBI_STACKTRACEINFO );
        }
    }else{
        if(!superClassConstructor){
            QString superClassName = qtjambi_class_name(env, generatedSuperclass).replace(QLatin1Char('$'), QLatin1Char('.'));
            JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class %1 cannot be registered as Qml type because its super class %2 is excluded.").arg(QString(javaName).replace(QLatin1Char('$'), QLatin1Char('.'))).arg(superClassName)) QTJAMBI_STACKTRACEINFO );
        }
    }
    return constructor;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterType
 * Signature: (Ljava/lang/Class;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterType__Ljava_lang_Class_2
  (JNIEnv * env, jclass, jclass clazz){
    env->EnsureLocalCapacity(300);
    jint _result{-1};
    QTJAMBI_TRY{
        int objectSize = int(qtjambi_extended_size_for_class(env, clazz));
        QString javaName = qtjambi_class_name(env, clazz);
        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        jobject method = qtjambi_find_QmlAttachedProperties(env, clazz);

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QQmlPrivate::RegisterType type = {
            /* int version */ 0,

            /*int typeId*/ registerQmlMetaType(javaName, meta_object),
            /*int listId*/ registerQmlListType(javaName),
            /*int objectSize*/ objectSize,
            /*void (*create)(void *)*/ nullptr,
            /*QString noCreationReason*/ QString(),

            /*const char *uri*/ nullptr,
            /*int versionMajor*/ 0,
            /*int versionMinor*/ 0,
            /*const char *elementName*/ nullptr,
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            /*int parserStatusCast*/ parserStatusCast(env, clazz),
            /*int valueSourceCast*/ valueSourceCast(env, clazz),
            /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

            /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
            /*const QMetaObject *extensionMetaObject*/ nullptr,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ 0
        };

#else
        QQmlPrivate::RegisterType type = {
            /* int structVersion */ 0,

            /*int typeId*/ QMetaType(registerQmlMetaType(javaName, meta_object)),
            /*int listId*/ QMetaType(registerQmlListType(javaName)),
            /*int objectSize*/ objectSize,
            /*void (*create)(void *,void *)*/ nullptr,
            /*void *userdata*/ nullptr,
            /*QString noCreationReason*/ QString(),

            /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,
            /*const char *uri*/ nullptr,
            /*QTypeRevision version*/ QTypeRevision::zero(),
            /*const char *elementName*/ nullptr,
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            /*int parserStatusCast*/ parserStatusCast(env, clazz),
            /*int valueSourceCast*/ valueSourceCast(env, clazz),
            /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

            /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
            /*const QMetaObject *extensionMetaObject*/ nullptr,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ QTypeRevision::zero()
        };

#endif
        _result = QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterTypeNotAvailable
 * Signature: (Ljava/lang/String;IILjava/lang/String;Ljava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterTypeNotAvailable
  (JNIEnv * env, jclass, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName, jstring message){
    jint _result{-1};
    QTJAMBI_TRY{
        QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();
        QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();
        QString _message = qtjambi_to_qstring(env, message);
        _result = qmlRegisterTypeNotAvailable(_uri.constData(), int(versionMajor), int(versionMinor), _qmlName.constData(), _message);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterUncreatableType
 * Signature: (Ljava/lang/Class;Ljava/lang/String;IILjava/lang/String;Ljava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterUncreatableType__Ljava_lang_Class_2Ljava_lang_String_2IILjava_lang_String_2Ljava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName, jstring reason){
    jint _result{-1};
    QTJAMBI_TRY{
      QString javaName = qtjambi_class_name(env, clazz);
      QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();
      QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();

      const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
      if(!meta_object){
          jclass closestClass = resolveClosestQtSuperclass(env, clazz);
          if(closestClass){
              const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
              meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
          }
      }
      jobject method = qtjambi_find_QmlAttachedProperties(env, clazz);

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
      QQmlPrivate::RegisterType type = {
          /* int version */ 1,

          /*int typeId*/ registerQmlMetaType(javaName, meta_object),
          /*int listId*/ registerQmlListType(javaName),
          /*int objectSize*/ 0,
          /*void (*create)(void *)*/ nullptr,
          /*QString noCreationReason*/ qtjambi_to_qstring(env, reason),

          /*const char *uri*/ _uri.constData(),
          /*int versionMajor*/ int(versionMajor),
          /*int versionMinor*/ int(versionMinor),
          /*const char *elementName*/ _qmlName.constData(),
          /*const QMetaObject *metaObject*/ meta_object,

          /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
          /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

          /*int parserStatusCast*/ parserStatusCast(env, clazz),
          /*int valueSourceCast*/ valueSourceCast(env, clazz),
          /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

          /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
          /*const QMetaObject *extensionMetaObject*/ nullptr,

          /*QQmlCustomParser *customParser*/ nullptr,
          /*int revision*/ 0
      };
#else
      QQmlPrivate::RegisterType type = {
          /* int structVersion */ 0,

          /*int typeId*/ QMetaType(registerQmlMetaType(javaName, meta_object)),
          /*int listId*/ QMetaType(registerQmlListType(javaName)),
          /*int objectSize*/ 0,
          /*void (*create)(void *,void *)*/ nullptr,
          /*void *userdata*/ nullptr,
          /*QString noCreationReason*/ qtjambi_to_qstring(env, reason),

          /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

          /*const char *uri*/ _uri.constData(),
          /*QTypeRevision version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
          /*const char *elementName*/ _qmlName.constData(),
          /*const QMetaObject *metaObject*/ meta_object,

          /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
          /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

          /*int parserStatusCast*/ parserStatusCast(env, clazz),
          /*int valueSourceCast*/ valueSourceCast(env, clazz),
          /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

          /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
          /*const QMetaObject *extensionMetaObject*/ nullptr,

          /*QQmlCustomParser *customParser*/ nullptr,
          /*int revision*/ QTypeRevision::zero()
      };
#endif
      _result = QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterUncreatableType
 * Signature: (Ljava/lang/Class;ILjava/lang/String;IILjava/lang/String;Ljava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterUncreatableType__Ljava_lang_Class_2ILjava_lang_String_2IILjava_lang_String_2Ljava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jint metaObjectRevision, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName, jstring reason){
    jint _result{-1};
    QTJAMBI_TRY{
      QString javaName = qtjambi_class_name(env, clazz);
      QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();
      QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();

      const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
      if(!meta_object){
          jclass closestClass = resolveClosestQtSuperclass(env, clazz);
          if(closestClass){
              const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
              meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
          }
      }
      jobject method = qtjambi_find_QmlAttachedProperties(env, clazz);

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
      QQmlPrivate::RegisterType type = {
          /* int version */ 1,

          /*int typeId*/ registerQmlMetaType(javaName, meta_object),
          /*int listId*/ registerQmlListType(javaName),
          /*int objectSize*/ 0,
          /*void (*create)(void *)*/ nullptr,
          /*QString noCreationReason*/ qtjambi_to_qstring(env, reason),

          /*const char *uri*/ _uri.constData(),
          /*int versionMajor*/ int(versionMajor),
          /*int versionMinor*/ int(versionMinor),
          /*const char *elementName*/ _qmlName.constData(),
          /*const QMetaObject *metaObject*/ meta_object,

          /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
          /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

          /*int parserStatusCast*/ parserStatusCast(env, clazz),
          /*int valueSourceCast*/ valueSourceCast(env, clazz),
          /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

          /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
          /*const QMetaObject *extensionMetaObject*/ nullptr,

          /*QQmlCustomParser *customParser*/ nullptr,
          /*int revision*/ int(metaObjectRevision)
      };
#else
      QQmlPrivate::RegisterType type = {
          /* int structVersion */ 0,

          /*int typeId*/ QMetaType(registerQmlMetaType(javaName, meta_object)),
          /*int listId*/ QMetaType(registerQmlListType(javaName)),
          /*int objectSize*/ 0,
          /*void (*create)(void *,void *)*/ nullptr,
          /*void *userdata*/ nullptr,
          /*QString noCreationReason*/ qtjambi_to_qstring(env, reason),

          /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

          /*const char *uri*/ _uri.constData(),
          /*QTypeRevision version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
          /*const char *elementName*/ _qmlName.constData(),
          /*const QMetaObject *metaObject*/ meta_object,

          /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
          /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

          /*int parserStatusCast*/ parserStatusCast(env, clazz),
          /*int valueSourceCast*/ valueSourceCast(env, clazz),
          /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

          /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
          /*const QMetaObject *extensionMetaObject*/ nullptr,

          /*QQmlCustomParser *customParser*/ nullptr,
          /*int revision*/ QTypeRevision::fromMajorVersion(metaObjectRevision)
      };
#endif
      _result = QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterType
 * Signature: (Ljava/lang/Class;Ljava/lang/String;IILjava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterType__Ljava_lang_Class_2Ljava_lang_String_2IILjava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName){
    env->EnsureLocalCapacity(300);
    jint _result{-1};
    QTJAMBI_TRY{
          size_t objectSize = qtjambi_extended_size_for_class(env, clazz);
          QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();
          QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();

          QString javaName = qtjambi_class_name(env, clazz);
          jmethodID constructor = findConstructor(env, clazz, javaName);

          const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
          if(!meta_object){
              jclass closestClass = resolveClosestQtSuperclass(env, clazz);
              if(closestClass){
                  const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                  meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
              }
          }
          jobject method = qtjambi_find_QmlAttachedProperties(env, clazz);
          int psCast = parserStatusCast(env, clazz);
          int vsCast = valueSourceCast(env, clazz);
          int viCast = valueInterceptorCast(env, clazz);

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
          QQmlPrivate::RegisterType type = {
              /* int version */ 1,

              /*int typeId*/ registerQmlMetaType(javaName, meta_object),
              /*int listId*/ registerQmlListType(javaName),
              /*int objectSize*/ int(objectSize),
              /*void (*create)(void *)*/ creatorFunction(env, meta_object, clazz, constructor, objectSize, psCast, vsCast, viCast),
              /*QString noCreationReason*/ QString(),

              /*const char *uri*/ _uri.constData(),
              /*int versionMajor*/ int(versionMajor),
              /*int versionMinor*/ int(versionMinor),
              /*const char *elementName*/ _qmlName.constData(),
              /*const QMetaObject *metaObject*/ meta_object,

              /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
              /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

              psCast,
              vsCast,
              viCast,

              /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
              /*const QMetaObject *extensionMetaObject*/ nullptr,

              /*QQmlCustomParser *customParser*/ nullptr,
              /*int revision*/ 0
          };
#else
          QQmlPrivate::RegisterType type = {
              /* int structVersion */ 0,

              /*int typeId*/ QMetaType(registerQmlMetaType(javaName, meta_object)),
              /*int listId*/ QMetaType(registerQmlListType(javaName)),
              /*int objectSize*/ int(objectSize),
              /*void (*create)(void *,void *)*/ &createQmlObject,
              /*void *userdata*/ creatorFunctionMetaData(env, meta_object, clazz, constructor, objectSize, psCast, vsCast, viCast),
              /*QString noCreationReason*/ QString(),

              /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

              /*const char *uri*/ _uri.constData(),
              /*QTypeRevision version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
              /*const char *elementName*/ _qmlName.constData(),
              /*const QMetaObject *metaObject*/ meta_object,

              /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
              /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

              psCast,
              vsCast,
              viCast,

              /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
              /*const QMetaObject *extensionMetaObject*/ nullptr,

              /*QQmlCustomParser *customParser*/ nullptr,
              /*int revision*/ QTypeRevision::zero()
          };
#endif
          _result = QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterType
 * Signature: (Ljava/lang/Class;ILjava/lang/String;IILjava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterType__Ljava_lang_Class_2ILjava_lang_String_2IILjava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jint metaObjectRevision, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName){
    env->EnsureLocalCapacity(300);
    jint _result{-1};
    QTJAMBI_TRY{
          size_t objectSize = qtjambi_extended_size_for_class(env, clazz);
          QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();
          QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();

          QString javaName = qtjambi_class_name(env, clazz);
          jmethodID constructor = findConstructor(env, clazz, javaName);

          const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
          if(!meta_object){
              jclass closestClass = resolveClosestQtSuperclass(env, clazz);
              if(closestClass){
                  const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                  meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
              }
          }
          jobject method = qtjambi_find_QmlAttachedProperties(env, clazz);
          int psCast = parserStatusCast(env, clazz);
          int vsCast = valueSourceCast(env, clazz);
          int viCast = valueInterceptorCast(env, clazz);

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
          QQmlPrivate::RegisterType type = {
              /* int version */ 1,

              /*int typeId*/ registerQmlMetaType(javaName, meta_object),
              /*int listId*/ registerQmlListType(javaName),
              /*int objectSize*/ int(objectSize),
              /*void (*create)(void *)*/ creatorFunction(env, meta_object, clazz, constructor, objectSize, psCast, vsCast, viCast),
              /*QString noCreationReason*/ QString(),

              /*const char *uri*/ _uri.constData(),
              /*int versionMajor*/ int(versionMajor),
              /*int versionMinor*/ int(versionMinor),
              /*const char *elementName*/ _qmlName.constData(),
              /*const QMetaObject *metaObject*/ meta_object,

              /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
              /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

              psCast,
              vsCast,
              viCast,

              /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
              /*const QMetaObject *extensionMetaObject*/ nullptr,

              /*QQmlCustomParser *customParser*/ nullptr,
              /*int revision*/ int(metaObjectRevision)
          };
#else
          QQmlPrivate::RegisterType type = {
              /* int structVersion */ 0,

              /*int typeId*/ QMetaType(registerQmlMetaType(javaName, meta_object)),
              /*int listId*/ QMetaType(registerQmlListType(javaName)),
              /*int objectSize*/ int(objectSize),
              /*void (*create)(void *,void *)*/ &createQmlObject,
              /*void *userdata*/ creatorFunctionMetaData(env, meta_object, clazz, constructor, objectSize, psCast, vsCast, viCast),
              /*QString noCreationReason*/ QString(),

              /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

              /*const char *uri*/ _uri.constData(),
              /*QTypeRevision version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
              /*const char *elementName*/ _qmlName.constData(),
              /*const QMetaObject *metaObject*/ meta_object,

              /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
              /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

              psCast,
              vsCast,
              viCast,

              /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
              /*const QMetaObject *extensionMetaObject*/ nullptr,

              /*QQmlCustomParser *customParser*/ nullptr,
              /*int revision*/ QTypeRevision::fromMajorVersion(metaObjectRevision)
          };
#endif
          _result = QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterRevision
 * Signature: (Ljava/lang/Class;ILjava/lang/String;II)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterRevision
  (JNIEnv * env, jclass, jclass clazz, jint metaObjectRevision, jstring uri, jint versionMajor, jint versionMinor){
    jint _result{-1};
    QTJAMBI_TRY{
        size_t objectSize = qtjambi_extended_size_for_class(env, clazz);
        QString javaName = qtjambi_class_name(env, clazz);
        jmethodID constructor = findConstructor(env, clazz, javaName);
        QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();

        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        jobject method = qtjambi_find_QmlAttachedProperties(env, clazz);
        int psCast = parserStatusCast(env, clazz);
        int vsCast = valueSourceCast(env, clazz);
        int viCast = valueInterceptorCast(env, clazz);

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QQmlPrivate::RegisterType type = {
            /* int version */ 1,

            /*int typeId*/ registerQmlMetaType(javaName, meta_object),
            /*int listId*/ registerQmlListType(javaName),
            /*int objectSize*/ int(objectSize),
            /*void (*create)(void *)*/ creatorFunction(env, meta_object, clazz, constructor, objectSize, psCast, vsCast, viCast),
            /*QString noCreationReason*/ QString(),

            /*const char *uri*/ _uri.constData(),
            /*int versionMajor*/ int(versionMajor),
            /*int versionMinor*/ int(versionMinor),
            /*const char *elementName*/ nullptr,
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            psCast,
            vsCast,
            viCast,

            /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
            /*const QMetaObject *extensionMetaObject*/ nullptr,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ int(metaObjectRevision)
        };
#else
        QQmlPrivate::RegisterType type = {
            /* int structVersion */ 0,

            /*int typeId*/ QMetaType(registerQmlMetaType(javaName, meta_object)),
            /*int listId*/ QMetaType(registerQmlListType(javaName)),
            /*int objectSize*/ int(objectSize),
            /*void (*create)(void *,void *)*/ &createQmlObject,
            /*void *userdata*/ creatorFunctionMetaData(env, meta_object, clazz, constructor, objectSize, psCast, vsCast, viCast),
            /*QString noCreationReason*/ QString(),

            /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

            /*const char *uri*/ _uri.constData(),
            /*QTypeRevision version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
            /*const char *elementName*/ nullptr,
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            psCast,
            vsCast,
            viCast,

            /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
            /*const QMetaObject *extensionMetaObject*/ nullptr,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ QTypeRevision::fromMajorVersion(metaObjectRevision)
        };
#endif
        _result = QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterExtendedType
 * Signature: (Ljava/lang/Class;Ljava/lang/Class;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterExtendedType__Ljava_lang_Class_2Ljava_lang_Class_2
  (JNIEnv * env, jclass, jclass clazz, jclass extendedClazz){
    jint _result{-1};
    QTJAMBI_TRY{
        QString javaName = qtjambi_class_name(env, clazz);
        QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
        jmethodID constructor(nullptr);
        QTJAMBI_TRY_ANY{
            constructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
        if(!constructor){
            JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
        }

        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        const QMetaObject *extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, nullptr);
        if(!extended_meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, extendedClazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, original_meta_object);
            }
        }
        jobject method = qtjambi_find_QmlAttachedProperties(env, clazz);

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QQmlPrivate::RegisterType type = {
            /* int version */ 0,

            /*int typeId*/ registerQmlMetaType(javaName, meta_object),
            /*int listId*/ registerQmlListType(javaName),
            /*int objectSize*/ 0,
            /*void (*create)(void *)*/ nullptr,
            /*QString noCreationReason*/ QString(),

            /*const char *uri*/ nullptr,
            /*int versionMajor*/ 0,
            /*int versionMinor*/ 0,
            /*const char *elementName*/ nullptr,
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            /*int parserStatusCast*/ parserStatusCast(env, clazz),
            /*int valueSourceCast*/ valueSourceCast(env, clazz),
            /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

            /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, constructor),
            /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ 0
        };
#else
        QQmlPrivate::RegisterType type = {
            /* int structVersion */ 0,

            /*int typeId*/ QMetaType(registerQmlMetaType(javaName, meta_object)),
            /*int listId*/ QMetaType(registerQmlListType(javaName)),
            /*int objectSize*/ 0,
            /*void (*create)(void *,void *)*/ nullptr,
            /*void *userdata*/ nullptr,
            /*QString noCreationReason*/ QString(),

            /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

            /*const char *uri*/ nullptr,
            /*int version*/ QTypeRevision::zero(),
            /*const char *elementName*/ nullptr,
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            /*int parserStatusCast*/ parserStatusCast(env, clazz),
            /*int valueSourceCast*/ valueSourceCast(env, clazz),
            /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

            /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, constructor),
            /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ QTypeRevision::zero()
        };
#endif
        _result = QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterExtendedType
 * Signature: (Ljava/lang/Class;Ljava/lang/Class;Ljava/lang/String;I)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterExtendedType__Ljava_lang_Class_2Ljava_lang_Class_2Ljava_lang_String_2I
  (JNIEnv * env, jclass, jclass clazz, jclass extendedClazz, jstring uri, jint versionMajor){
    jint _result{-1};
    QTJAMBI_TRY{
        QString _uri = qtjambi_to_qstring(env, uri);
        QString javaName = qtjambi_class_name(env, clazz);
        QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
        jmethodID constructor(nullptr);
        QTJAMBI_TRY_ANY{
            constructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
        if(!constructor){
            JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
        }

        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        const QMetaObject *extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, nullptr);
        if(!extended_meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, extendedClazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, original_meta_object);
            }
        }
        jobject method = qtjambi_find_QmlAttachedProperties(env, clazz);

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QQmlPrivate::RegisterType type = {
            /* int version */ 0,

            /*int typeId*/ registerQmlMetaType(javaName, meta_object),
            /*int listId*/ registerQmlListType(javaName),
            /*int objectSize*/ 0,
            /*void (*create)(void *)*/ nullptr,
            /*QString noCreationReason*/ QString(),

            /*const char *uri*/ qPrintable(_uri),
            /*int versionMajor*/ versionMajor,
            /*int versionMinor*/ 0,
            /*const char *elementName*/ nullptr,
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            /*int parserStatusCast*/ parserStatusCast(env, clazz),
            /*int valueSourceCast*/ valueSourceCast(env, clazz),
            /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

            /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, constructor),
            /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ 0
        };
#else
        QQmlPrivate::RegisterType type = {
            /* int structVersion */ 0,

            /*int typeId*/ QMetaType(registerQmlMetaType(javaName, meta_object)),
            /*int listId*/ QMetaType(registerQmlListType(javaName)),
            /*int objectSize*/ 0,
            /*void (*create)(void *,void *)*/ nullptr,
            /*void *userdata*/ nullptr,
            /*QString noCreationReason*/ QString(),

            /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

            /*const char *uri*/ qPrintable(_uri),
            /*int version*/ QTypeRevision::fromMajorVersion(versionMajor),
            /*const char *elementName*/ nullptr,
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            /*int parserStatusCast*/ parserStatusCast(env, clazz),
            /*int valueSourceCast*/ valueSourceCast(env, clazz),
            /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

            /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, constructor),
            /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ QTypeRevision::zero()
        };
#endif
        _result = QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterExtendedType
 * Signature: (Ljava/lang/Class;Ljava/lang/Class;Ljava/lang/String;IILjava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterExtendedType__Ljava_lang_Class_2Ljava_lang_Class_2Ljava_lang_String_2IILjava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jclass extendedClazz, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName){
    jint _result{-1};
    QTJAMBI_TRY{
        size_t objectSize = qtjambi_extended_size_for_class(env, clazz);

        QString javaName = qtjambi_class_name(env, clazz);
        QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
        jmethodID constructor = findConstructor(env, clazz, javaName);
        jmethodID econstructor(nullptr);
        QTJAMBI_TRY_ANY{
            econstructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
        if(!econstructor){
            JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
        }

        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        const QMetaObject *extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, nullptr);
        if(!extended_meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, extendedClazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, original_meta_object);
            }
        }
        jclass declClass = clazz;
        jobject method = qtjambi_find_QmlAttachedProperties(env, declClass);
        if(!method){
            declClass = extendedClazz;
            method = qtjambi_find_QmlAttachedProperties(env, declClass);
        }

        QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();
        QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();
        int psCast = parserStatusCast(env, clazz);
        int vsCast = valueSourceCast(env, clazz);
        int viCast = valueInterceptorCast(env, clazz);

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QQmlPrivate::RegisterType type = {
            /* int version */ 0,

            /*int typeId*/ registerQmlMetaType(javaName, meta_object),
            /*int listId*/ registerQmlListType(javaName),
            /*int objectSize*/ int(objectSize),
            /*void (*create)(void *)*/ creatorFunction(env, meta_object, clazz, constructor, objectSize, psCast, vsCast, viCast),
            /*QString noCreationReason*/ QString(),

            /*const char *uri*/ _uri.constData(),
            /*int versionMajor*/ int(versionMajor),
            /*int versionMinor*/ int(versionMinor),
            /*const char *elementName*/ _qmlName.isEmpty() ? nullptr : _qmlName.constData(),
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, declClass, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            psCast,
            vsCast,
            viCast,

            /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, econstructor),
            /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ 0
        };
#else
        QQmlPrivate::RegisterType type = {
            /* int structVersion */ 0,

            /*int typeId*/ QMetaType(registerQmlMetaType(javaName, meta_object)),
            /*int listId*/ QMetaType(registerQmlListType(javaName)),
            /*int objectSize*/ int(objectSize),
            /*void (*create)(void *,void *)*/ &createQmlObject,
            /*void *userdata*/ creatorFunctionMetaData(env, meta_object, clazz, constructor, objectSize, psCast, vsCast, viCast),
            /*QString noCreationReason*/ QString(),

            /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

            /*const char *uri*/ _uri.constData(),
            /*int version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
            /*const char *elementName*/ _qmlName.isEmpty() ? nullptr : _qmlName.constData(),
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, declClass, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            psCast,
            vsCast,
            viCast,

            /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, econstructor),
            /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ QTypeRevision::zero()
        };
#endif
        _result = QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterInterface
 * Signature: (Ljava/lang/Class;Ljava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterInterface1
  (JNIEnv * env, jclass, jclass clazz, jstring name){
    jint _result{-1};
    QTJAMBI_TRY{
        QString typeName = qtjambi_to_qstring(env, name);
        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        const char* iid = getInterfaceIID(env, clazz);
        if(!iid){
            iid = registerInterfaceID(env, clazz);
        }

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QQmlPrivate::RegisterInterface qmlInterface = {
            /* int version */ 1,

            /* int typeId */ registerQmlMetaType(typeName, meta_object),
            /* int listId */ registerQmlListType(typeName),

            /* const char *iid */ iid,
            
            /* const char *uri */ "",
            /* int versionMajor */ 0
        };
#else
        QQmlPrivate::RegisterInterface qmlInterface = {
            /* int structVersion */ 0,

            /* QMetaType typeId */ QMetaType(registerQmlMetaType(typeName, meta_object)),
            /* QMetaType listId */ QMetaType(registerQmlListType(typeName)),

            /* const char *iid */ iid,

            /* const char *uri */ "",
            /* int versionMajor */ QTypeRevision::zero()
        };
#endif
        _result = QQmlPrivate::qmlregister(QQmlPrivate::InterfaceRegistration, &qmlInterface);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterInterface
 * Signature: (Ljava/lang/Class;Ljava/lang/String;I)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterInterface2
  (JNIEnv * env, jclass, jclass clazz, jstring uri, jint versionMajor){
    jint _result{-1};
    QTJAMBI_TRY{
        QString _uri = qtjambi_to_qstring(env, uri);
        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        QString typeName;
        if(meta_object){
            typeName = meta_object->className();
        }

        const char* iid = getInterfaceIID(env, clazz);
        if(!iid){
            iid = registerInterfaceID(env, clazz);
        }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QQmlPrivate::RegisterInterface qmlInterface = {
            /* int version */ 1,

            /* int typeId */ registerQmlMetaType(typeName, meta_object),
            /* int listId */ registerQmlListType(typeName),

            /* const char *iid */ iid,
            
            /* const char *uri */ qPrintable(_uri),
            /* int versionMajor */ versionMajor
        };

#else
        QQmlPrivate::RegisterInterface qmlInterface = {
            /* int structVersion */ 0,

            /* QMetaType typeId */ QMetaType(registerQmlMetaType(typeName, meta_object)),
            /* QMetaType listId */ QMetaType(registerQmlListType(typeName)),

            /* const char *iid */ iid,

            /* const char *uri */ qPrintable(_uri),
            /* int versionMajor */ QTypeRevision::fromMajorVersion(versionMajor)
        };
#endif
        _result = QQmlPrivate::qmlregister(QQmlPrivate::InterfaceRegistration, &qmlInterface);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlAttachedPropertiesObject
 * Signature: (Lio/qt/core/QObject;Z)Lio/qt/core/QObject;
 */
extern "C" Q_DECL_EXPORT jobject JNICALL Java_io_qt_qml_QtQml_qmlAttachedPropertiesObject
  (JNIEnv * env, jclass, jclass clazz, jobject object, jboolean create){
    jobject _result{nullptr};
    QTJAMBI_TRY{
        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        static int idx = -1;
        QObject* result = QtQml::qmlAttachedPropertiesObject(&idx, qtjambi_to_qobject(env, object), meta_object, create);
#else
        QObject* obj = qtjambi_to_qobject(env, object);
        static const auto func = qmlAttachedPropertiesFunction(nullptr, meta_object);
        QObject* result = qmlAttachedPropertiesObject(obj, func, create);
#endif
        _result = qtjambi_from_QObject(env, result);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterBaseTypes
 * Signature: (Ljava/lang/String;II)V
 */
extern "C" Q_DECL_EXPORT void JNICALL Java_io_qt_qml_QtQml_qmlRegisterBaseTypes
  (JNIEnv * env, jclass, jstring uri, jint versionMajor, jint versionMinor){
    QTJAMBI_TRY{
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
        qmlRegisterBaseTypes(qPrintable(qtjambi_to_qstring(env, uri)), int(versionMajor), int(versionMinor));
#else
        Q_UNUSED(uri)
        Q_UNUSED(versionMajor)
        Q_UNUSED(versionMinor)
        JavaException::raiseQNoImplementationException(env, "qmlRegisterBaseTypes() is no longer available since Qt 5.14" QTJAMBI_STACKTRACEINFO);
#endif
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterAnonymousType
 * Signature: (Ljava/lang/String;I)V
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterAnonymousType
  (JNIEnv * env, jclass, jclass clazz, jstring uri, jint versionMajor){
    jint _result{-1};
    QTJAMBI_TRY{
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
        Q_UNUSED(clazz)
        Q_UNUSED(uri)
        Q_UNUSED(versionMajor)
        JavaException::raiseQNoImplementationException(env, "qmlRegisterAnonymousType() is not available in Qt versions older than 5.14" QTJAMBI_STACKTRACEINFO);
#else
        QString javaName = qtjambi_class_name(env, clazz);
        QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();

        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        jobject method = qtjambi_find_QmlAttachedProperties(env, clazz);

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QQmlPrivate::RegisterType type = {
            /* int version */ 0,

            /*int typeId*/ registerQmlMetaType(javaName, meta_object),
            /*int listId*/ registerQmlListType(javaName),
            /*int objectSize*/ 0,
            /*void (*create)(void *)*/ nullptr,
            /*QString noCreationReason*/ QString(),

            /*const char *uri*/ _uri.constData(),
            /*int versionMajor*/ int(versionMajor),
            /*int versionMinor*/ 0,
            /*const char *elementName*/ nullptr,
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            /*int parserStatusCast*/ parserStatusCast(env, clazz),
            /*int valueSourceCast*/ valueSourceCast(env, clazz),
            /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

            /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
            /*const QMetaObject *extensionMetaObject*/ nullptr,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ 0
        };
#else
        QQmlPrivate::RegisterType type = {
            /* int structVersion */ 0,

            /* QMetaType typeId */ QMetaType(registerQmlMetaType(javaName, meta_object)),
            /* QMetaType listId */ QMetaType(registerQmlListType(javaName)),

            /*int objectSize*/ 0,
            /*void (*create)(void *,void *)*/ nullptr,
            /*void *userdata*/ nullptr,
            /*QString noCreationReason*/ QString(),

            /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

            /*const char *uri*/ _uri.constData(),
            /*int versionMajor*/ QTypeRevision::fromMajorVersion(versionMajor),
            /*const char *elementName*/ nullptr,
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            /*int parserStatusCast*/ parserStatusCast(env, clazz),
            /*int valueSourceCast*/ valueSourceCast(env, clazz),
            /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

            /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
            /*const QMetaObject *extensionMetaObject*/ nullptr,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ QTypeRevision::zero()
        };
#endif
        _result = QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
#endif
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterType
 * Signature: (Lio/qt/core/QUrl;Ljava/lang/String;IILjava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterType__Lio_qt_core_QUrl_2Ljava_lang_String_2IILjava_lang_String_2
  (JNIEnv * env, jclass, jobject url, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName){
    env->EnsureLocalCapacity(300);
    jint _result{-1};
    QTJAMBI_TRY{
        QUrl* _url = qtjambi_cast<QUrl*>(env, url);
        QString _qmlName = qtjambi_to_qstring(env, qmlName);
        _result = qmlRegisterType(_url ? *_url : QUrl(), qPrintable(qtjambi_to_qstring(env, uri)), int(versionMajor), int(versionMinor), _qmlName.isEmpty() ? nullptr : qPrintable(_qmlName));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlTypeId
 * Signature: (Ljava/lang/String;IILjava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlTypeId__Ljava_lang_String_2IILjava_lang_String_2
  (JNIEnv * env, jclass, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName){
    env->EnsureLocalCapacity(300);
    jint _result{-1};
    QTJAMBI_TRY{
        QString _qmlName = qtjambi_to_qstring(env, qmlName);
        _result = qmlTypeId(qPrintable(qtjambi_to_qstring(env, uri)), int(versionMajor), int(versionMinor), _qmlName.isEmpty() ? nullptr : qPrintable(_qmlName));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlProtectModule
 * Signature: (Ljava/lang/String;I)Z
 */
extern "C" Q_DECL_EXPORT jboolean JNICALL Java_io_qt_qml_QtQml_qmlProtectModule
  (JNIEnv * env, jclass, jstring uri, jint majVersion){
    jboolean _result{false};
    QTJAMBI_TRY{
        _result = qmlProtectModule(qPrintable(qtjambi_to_qstring(env, uri)), int(majVersion));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlClearTypeRegistrations
 * Signature: ()V
 */
extern "C" Q_DECL_EXPORT void JNICALL Java_io_qt_qml_QtQml_qmlClearTypeRegistrations
  (JNIEnv *, jclass){
    qmlClearTypeRegistrations();
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterModule
 * Signature: (Ljava/lang/String;II)V
 */
extern "C" Q_DECL_EXPORT void Java_io_qt_qml_QtQml_qmlRegisterModule__Ljava_lang_String_2II
  (JNIEnv * env, jclass, jstring uri, jint versionMajor, jint versionMinor){
    QTJAMBI_TRY{
        QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();
        qmlRegisterModule(_uri.constData(), int(versionMajor), int(versionMinor));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterExtendedUncreatableType
 * Signature: (Ljava/lang/Class;Ljava/lang/Class;Ljava/lang/String;IILjava/lang/String;Ljava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterExtendedUncreatableType__Ljava_lang_Class_2Ljava_lang_Class_2Ljava_lang_String_2IILjava_lang_String_2Ljava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jclass extendedClazz, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName, jstring reason){
    jint _result{-1};
    QTJAMBI_TRY{
        QString javaName = qtjambi_class_name(env, clazz);
        QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
        jmethodID constructor(nullptr);
        QTJAMBI_TRY_ANY{
            constructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
        if(!constructor){
            JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
        }

        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        const QMetaObject *extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, nullptr);
        if(!extended_meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, extendedClazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, original_meta_object);
            }
        }
        jclass declClass = clazz;
        jobject method = qtjambi_find_QmlAttachedProperties(env, declClass);
        if(!method){
            declClass = extendedClazz;
            method = qtjambi_find_QmlAttachedProperties(env, declClass);
        }

        QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();
        QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QQmlPrivate::RegisterType type = {
            /* int version */ 0,

            /*int typeId*/ registerQmlMetaType(javaName, meta_object),
            /*int listId*/ registerQmlListType(javaName),
            /*int objectSize*/ 0,
            /*void (*create)(void *)*/ nullptr,
            /*QString noCreationReason*/ qtjambi_to_qstring(env, reason),

            /*const char *uri*/ _uri.constData(),
            /*int versionMajor*/ int(versionMajor),
            /*int versionMinor*/ int(versionMinor),
            /*const char *elementName*/ _qmlName.isEmpty() ? nullptr : _qmlName.constData(),
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, declClass, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            /*int parserStatusCast*/ parserStatusCast(env, clazz),
            /*int valueSourceCast*/ valueSourceCast(env, clazz),
            /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

            /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, constructor),
            /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ 0
        };
#else
        QQmlPrivate::RegisterType type = {
            /* int structVersion */ 0,

            /* QMetaType typeId */ QMetaType(registerQmlMetaType(javaName, meta_object)),
            /* QMetaType listId */ QMetaType(registerQmlListType(javaName)),
            /*int objectSize*/ 0,
            /*void (*create)(void *,void *)*/ nullptr,
            /*void *userdata*/ nullptr,
            /*QString noCreationReason*/ qtjambi_to_qstring(env, reason),

            /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

            /*const char *uri*/ _uri.constData(),
            /*QTypeRevision version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
            /*const char *elementName*/ _qmlName.isEmpty() ? nullptr : _qmlName.constData(),
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, declClass, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            /*int parserStatusCast*/ parserStatusCast(env, clazz),
            /*int valueSourceCast*/ valueSourceCast(env, clazz),
            /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

            /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, constructor),
            /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ QTypeRevision::zero()
        };
#endif
        _result = QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterExtendedUncreatableType
 * Signature: (Ljava/lang/Class;Ljava/lang/Class;ILjava/lang/String;IILjava/lang/String;Ljava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterExtendedUncreatableType__Ljava_lang_Class_2Ljava_lang_Class_2ILjava_lang_String_2IILjava_lang_String_2Ljava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jclass extendedClazz, jint metaObjectRevision, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName, jstring reason){
    jint _result{-1};
    QTJAMBI_TRY{
        QString javaName = qtjambi_class_name(env, clazz);
        QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
        jmethodID constructor(nullptr);
        QTJAMBI_TRY_ANY{
            constructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
        if(!constructor){
            JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
        }

        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        const QMetaObject *extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, nullptr);
        if(!extended_meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, extendedClazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, original_meta_object);
            }
        }
        jclass declClass = clazz;
        jobject method = qtjambi_find_QmlAttachedProperties(env, declClass);
        if(!method){
            declClass = extendedClazz;
            method = qtjambi_find_QmlAttachedProperties(env, declClass);
        }

        QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();
        QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QQmlPrivate::RegisterType type = {
            /* int version */ 0,

            /*int typeId*/ registerQmlMetaType(javaName, meta_object),
            /*int listId*/ registerQmlListType(javaName),
            /*int objectSize*/ 0,
            /*void (*create)(void *)*/ nullptr,
            /*QString noCreationReason*/ qtjambi_to_qstring(env, reason),

            /*const char *uri*/ _uri.constData(),
            /*int versionMajor*/ int(versionMajor),
            /*int versionMinor*/ int(versionMinor),
            /*const char *elementName*/ _qmlName.isEmpty() ? nullptr : _qmlName.constData(),
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, declClass, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            /*int parserStatusCast*/ parserStatusCast(env, clazz),
            /*int valueSourceCast*/ valueSourceCast(env, clazz),
            /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

            /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, constructor),
            /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ int(metaObjectRevision)
        };
#else
        QQmlPrivate::RegisterType type = {
            /* int structVersion */ 0,

            /* QMetaType typeId */ QMetaType(registerQmlMetaType(javaName, meta_object)),
            /* QMetaType listId */ QMetaType(registerQmlListType(javaName)),
            /*int objectSize*/ 0,
            /*void (*create)(void *,void *)*/ nullptr,
            /*void *userdata*/ nullptr,
            /*QString noCreationReason*/ qtjambi_to_qstring(env, reason),

            /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

            /*const char *uri*/ _uri.constData(),
            /*QTypeRevision version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
            /*const char *elementName*/ _qmlName.isEmpty() ? nullptr : _qmlName.constData(),
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, declClass, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            /*int parserStatusCast*/ parserStatusCast(env, clazz),
            /*int valueSourceCast*/ valueSourceCast(env, clazz),
            /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

            /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, constructor),
            /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ QTypeRevision::fromMajorVersion(metaObjectRevision)
        };
#endif
        _result = QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterSingletonType
 * Signature: (Lio/qt/core/QUrl;Ljava/lang/String;IILjava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterSingletonType__Lio_qt_core_QUrl_2Ljava_lang_String_2IILjava_lang_String_2
  (JNIEnv * env, jclass, jobject url, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName){
    jint _result{-1};
    QTJAMBI_TRY{
        QUrl _url = qtjambi_cast<QUrl>(env, url);
        QString _qmlName = qtjambi_to_qstring(env, qmlName);
        _result = qmlRegisterSingletonType(_url, qPrintable(qtjambi_to_qstring(env, uri)), int(versionMajor), int(versionMinor), _qmlName.isEmpty() ? nullptr : qPrintable(_qmlName));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterSingletonType
 * Signature: (Ljava/lang/String;IILjava/lang/String;Lio/qt/qml/QtQml$ValueCallback;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterSingletonType__Ljava_lang_String_2IILjava_lang_String_2Lio_qt_qml_QtQml_00024ValueCallback_2
  (JNIEnv * env, jclass, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName, jobject callback){
    jint _result{-1};
    QTJAMBI_TRY{
        QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();
        QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QQmlPrivate::RegisterSingletonType api = {
            /* int version */     0,
            /* const char *uri */ _uri.constData(),
            /* int versionMajor */ int(versionMajor),
            /* int versionMinor */ int(versionMinor),
            /* const char *typeName */ _qmlName.isEmpty() ? nullptr : _qmlName.constData(),
            /* scriptApi */ qtjambi_to_functional< QJSValue (*)(QQmlEngine *, QJSEngine *) >(env, callback, "io/qt/qml/QtQml$ValueCallback"),
            /* qobjectApi */ nullptr,
            /* const QMetaObject *instanceMetaObject */ nullptr,
            /* int typeId */ 0,
            /* int revision */ 0
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
            ,/* generalizedQobjectApi */ {}
#endif
        };
#else
        QQmlPrivate::RegisterSingletonType api = {
            /* int structVersion */     0,
            /* const char *uri */ _uri.constData(),
            /*QTypeRevision version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
            /* const char *typeName */ _qmlName.isEmpty() ? nullptr : _qmlName.constData(),
            /* scriptApi */ qtjambi_to_functional< std::function<QJSValue(QQmlEngine *, QJSEngine *)> >(env, callback, "io/qt/qml/QtQml$ValueCallback"),
            /* qobjectApi */ nullptr,
            /* const QMetaObject *instanceMetaObject */ nullptr,
            /* QMetaType typeId */ QMetaType(QMetaType::UnknownType),
            /* extensionObjectCreate */ nullptr,
            /* extensionMetaObject */ nullptr,
            /* int revision */ QTypeRevision::zero()
        };
#endif
        _result = QQmlPrivate::qmlregister(QQmlPrivate::SingletonRegistration, &api);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterSingletonType
 * Signature: (Ljava/lang/Class;Ljava/lang/String;IILjava/lang/String;Lio/qt/qml/QtQml$ObjectCallback;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterSingletonType__Ljava_lang_Class_2Ljava_lang_String_2IILjava_lang_String_2Lio_qt_qml_QtQml_00024ObjectCallback_2
  (JNIEnv * env, jclass, jclass clazz, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName, jobject callback){
    jint _result{-1};
    QTJAMBI_TRY{
        QString javaName = qtjambi_class_name(env, clazz);

        QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();
        QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();
        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QQmlPrivate::RegisterSingletonType api = {
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
            /* int version */     3,
#else
            /* int version */     2,
#endif
            /* const char *uri */ _uri.constData(),
            /* int versionMajor */ int(versionMajor),
            /* int versionMinor */ int(versionMinor),
            /* const char *typeName */ _qmlName.isEmpty() ? nullptr : _qmlName.constData(),
            /* scriptApi */ nullptr,
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
            /* qobjectApi */ nullptr,
#else
            /* qobjectApi */ qtjambi_to_functional< QObject* (*)(QQmlEngine *, QJSEngine *) >(env, callback, "io/qt/qml/QtQml$ObjectCallback"),
#endif
            /* const QMetaObject *instanceMetaObject */ meta_object,
            /* int typeId */ registerQmlMetaType(javaName, meta_object),
            /* int revision */ 0
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
                ,/* generalizedQobjectApi */ qtjambi_to_functional< std::function<QObject*(QQmlEngine *, QJSEngine *)> >(env, callback, "io/qt/qml/QtQml$ObjectCallback")
#endif
        };
#else
        QQmlPrivate::RegisterSingletonType api = {
            /* int structVersion */     0,
            /* const char *uri */ _uri.constData(),
            /*QTypeRevision version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
            /* const char *typeName */ _qmlName.isEmpty() ? nullptr : _qmlName.constData(),
            /* scriptApi */ nullptr,
            /* qobjectApi */ qtjambi_to_functional< std::function<QObject*(QQmlEngine *, QJSEngine *)> >(env, callback, "io/qt/qml/QtQml$ObjectCallback"),
            /* const QMetaObject *instanceMetaObject */ meta_object,
            /* QMetaType typeId */ QMetaType(registerQmlMetaType(javaName, meta_object)),
            /* extensionObjectCreate */ nullptr,
            /* extensionMetaObject */ nullptr,
            /* int revision */ QTypeRevision::zero()
        };
#endif
        _result = QQmlPrivate::qmlregister(QQmlPrivate::SingletonRegistration, &api);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterUncreatableClass
 * Signature: (Ljava/lang/Class;Ljava/lang/String;IILjava/lang/String;Ljava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterUncreatableClass__Ljava_lang_Class_2Ljava_lang_String_2IILjava_lang_String_2Ljava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName, jstring reason){
    jint _result{-1};
    QTJAMBI_TRY{
      QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();
      QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();

      const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
      if(!meta_object){
          jclass closestClass = resolveClosestQtSuperclass(env, clazz);
          if(closestClass){
              const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
              meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
          }
      }
      _result = !meta_object ? -1 : qmlRegisterUncreatableMetaObject(*meta_object,
                                              /*const char *uri*/ _uri.constData(),
                                              /*int versionMajor*/ int(versionMajor),
                                              /*int versionMinor*/ int(versionMinor),
                                              /*const char *elementName*/ _qmlName.isEmpty() ? nullptr : _qmlName.constData(),
                                              /*QString noCreationReason*/ qtjambi_to_qstring(env, reason));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jlong JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlIncubationController_00024WhileFlag_create)
(JNIEnv *, jclass, jboolean flag)
{
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
    return jlong(new volatile bool(flag));
#else
    return jlong(new std::atomic<bool>(flag));
#endif
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlIncubationController_00024WhileFlag_destroy)
(JNIEnv *, jclass, jlong ptr)
{
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
    if(ptr) delete reinterpret_cast<volatile bool*>(ptr);
#else
    if(ptr) delete reinterpret_cast<std::atomic<bool>*>(ptr);
#endif
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlIncubationController_00024WhileFlag_set)
(JNIEnv *, jclass, jlong ptr, jboolean flag)
{
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
    if(ptr) *reinterpret_cast<volatile bool*>(ptr) = flag;
#else
    if(ptr) *reinterpret_cast<std::atomic<bool>*>(ptr) = flag;
#endif
}

// QQmlEngine::singletonInstance(int)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlEngine_singletonInstance__JI)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId, jint qmlTypeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QQmlEngine::singletonInstance(int)")
    jobject result{nullptr};
    QTJAMBI_TRY {
        QQmlEngine *__qt_this = qtjambi_object_from_nativeId<QQmlEngine>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QJSValue value = __qt_this->singletonInstance<QJSValue>(qmlTypeId);
        result = qtjambi_cast<jobject>(__jni_env, value);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// qjsEngine
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QtQml_qjsEngine)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __object_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "qjsEngine")
    jobject result{nullptr};
    QTJAMBI_TRY {
        QObject *object = qtjambi_object_from_nativeId<QObject>(__object_nativeId);
        result = qtjambi_cast<jobject>(__jni_env, qjsEngine(object));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
using ConvertVariant = bool(QJSEngine::*)(const QVariant &value, QMetaType metaType, void *ptr);
template <>
constexpr ConvertVariant qjsvalue_cast<ConvertVariant>(const QJSValue &)
{
    return &QJSEngine::convertVariant;
}
// QJSEngine::fromVariant
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QJSEngine_fromVariant)
(JNIEnv *env,
 jclass,
 QtJambiNativeID __this_nativeId, jobject jVariant, QtJambiNativeID metaTypeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QJSEngine::fromVariant")
    jobject result{nullptr};
    QTJAMBI_TRY {
        QJSEngine *__qt_this = qtjambi_object_from_nativeId<QJSEngine>(__this_nativeId);
        qtjambi_check_resource(env, __qt_this);
        QVariant value = qtjambi_to_qvariant(env, jVariant);
        QMetaType targetType = qtjambi_object_from_nativeId_deref<QMetaType>(env, metaTypeId);
        if (value.metaType()==targetType){
            result = jVariant;
        }else{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0) && QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
            if(!targetType.iface()->defaultCtr){
                JavaException::raiseRuntimeException(env, qPrintable(QString("Unable to convert to meta type %1 due to missing default constructor.").arg(QLatin1String(targetType.name()))) QTJAMBI_STACKTRACEINFO );
            }
            if(!targetType.iface()->copyCtr){
                JavaException::raiseRuntimeException(env, qPrintable(QString("Unable to convert to meta type %1 due to missing copy constructor.").arg(QLatin1String(targetType.name()))) QTJAMBI_STACKTRACEINFO );
            }
#endif
            QVariant t(targetType, nullptr);
            ConvertVariant convertVariant = qjsvalue_cast<ConvertVariant>(QJSValue{});
            if ((__qt_this->*convertVariant)(value, targetType, t.data())){
                result = qtjambi_from_qvariant(env, t);
            }else{
                QMetaType::convert(value.metaType(), value.constData(), targetType, t.data());
                result = qtjambi_from_qvariant(env, t);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
using ConvertV2 = bool (*)(const QJSValue &value, QMetaType metaType, void *ptr);
template <>
constexpr ConvertV2 qjsvalue_cast<ConvertV2>(const QJSValue &)
{
    return &QJSEngine::convertV2;
}
// QJSEngine::fromScriptValue
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QJSEngine_fromScriptValue)
(JNIEnv *env,
 jclass,
 QtJambiNativeID __this_nativeId, QtJambiNativeID jmanaged, QtJambiNativeID metaTypeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QJSEngine::fromScriptValue")
    jobject result{nullptr};
    QTJAMBI_TRY {
        QJSEngine *__qt_this = qtjambi_object_from_nativeId<QJSEngine>(__this_nativeId);
        qtjambi_check_resource(env, __qt_this);
        const QJSValue& value = qtjambi_object_from_nativeId_deref<QJSValue>(env, jmanaged);
        QMetaType targetType = qtjambi_object_from_nativeId_deref<QMetaType>(env, metaTypeId);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0) && QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
        if(!targetType.iface()->defaultCtr){
            JavaException::raiseRuntimeException(env, qPrintable(QString("Unable to convert to meta type %1 due to missing default constructor.").arg(QLatin1String(targetType.name()))) QTJAMBI_STACKTRACEINFO );
        }
        if(!targetType.iface()->copyCtr){
            JavaException::raiseRuntimeException(env, qPrintable(QString("Unable to convert to meta type %1 due to missing copy constructor.").arg(QLatin1String(targetType.name()))) QTJAMBI_STACKTRACEINFO );
        }
#endif
        ConvertV2 convertV2 = qjsvalue_cast<ConvertV2>(QJSValue{});
        QVariant t(targetType, nullptr);
        if (convertV2(value, targetType, t.data())){
            result = qtjambi_from_qvariant(env, t);
        }else if (value.isVariant()){
            t = value.toVariant();
            t.convert(targetType);
            result = qtjambi_from_qvariant(env, t);
        }else{
            result = qtjambi_from_qvariant(env, QVariant(targetType, nullptr));
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

using Create = QJSValue(QJSEngine::*)(QMetaType type, const void *ptr);
template <>
constexpr Create qjsvalue_cast<Create>(const QJSValue &)
{
    return &QJSEngine::create;
}
// QJSEngine::toScriptValue
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QJSEngine_toScriptValue)
(JNIEnv *env,
 jclass,
 QtJambiNativeID __this_nativeId, jobject object)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QJSEngine::toScriptValue")
    jobject result{nullptr};
    QTJAMBI_TRY {
        QJSEngine *__qt_this = qtjambi_object_from_nativeId<QJSEngine>(__this_nativeId);
        qtjambi_check_resource(env, __qt_this);
        QVariant variant = qtjambi_to_qvariant(env, object);
        Create create = qjsvalue_cast<Create>(QJSValue{});
        QJSValue jsval = (__qt_this->*create)(variant.metaType(), variant.data());
        result = qtjambi_cast<jobject>(env, jsval);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 1, 0)
using ConvertManaged = bool(*)(const QJSManagedValue &value, QMetaType type, void *ptr);
template <>
constexpr ConvertManaged qjsvalue_cast<ConvertManaged>(const QJSValue &)
{
    return &QJSEngine::convertManaged;
}
// QJSEngine::fromManagedValue
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QJSEngine_fromManagedValue)
(JNIEnv *env,
 jclass,
 QtJambiNativeID __this_nativeId, QtJambiNativeID jmanaged, QtJambiNativeID metaTypeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QJSEngine::fromManagedValue")
    jobject result{nullptr};
    QTJAMBI_TRY {
        QJSEngine *__qt_this = qtjambi_object_from_nativeId<QJSEngine>(__this_nativeId);
        qtjambi_check_resource(env, __qt_this);
        const QJSManagedValue& value = qtjambi_object_from_nativeId_deref<QJSManagedValue>(env, jmanaged);
        QMetaType targetType = qtjambi_object_from_nativeId_deref<QMetaType>(env, metaTypeId);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0) && QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
        if(!targetType.iface()->defaultCtr){
            JavaException::raiseRuntimeException(env, qPrintable(QString("Unable to convert to meta type %1 due to missing default constructor.").arg(QLatin1String(targetType.name()))) QTJAMBI_STACKTRACEINFO );
        }
        if(!targetType.iface()->copyCtr){
            JavaException::raiseRuntimeException(env, qPrintable(QString("Unable to convert to meta type %1 due to missing copy constructor.").arg(QLatin1String(targetType.name()))) QTJAMBI_STACKTRACEINFO );
        }
#endif
        ConvertManaged convertManaged = qjsvalue_cast<ConvertManaged>(QJSValue{});
        QVariant t(targetType, nullptr);
        if (convertManaged(value, targetType, t.data())){
            result = qtjambi_from_qvariant(env, t);
        }else{
            t = value.toVariant();
            t.convert(targetType);
            result = qtjambi_from_qvariant(env, t);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

using CreateManaged = QJSManagedValue(QJSEngine::*)(QMetaType type, const void *ptr);
template <>
constexpr CreateManaged qjsvalue_cast<CreateManaged>(const QJSValue &)
{
    return &QJSEngine::createManaged;
}
// QJSEngine::toManagedValue
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QJSEngine_toManagedValue)
(JNIEnv *env,
 jclass,
 QtJambiNativeID __this_nativeId, jobject object)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QJSEngine::toManagedValue")
    jobject result{nullptr};
    QTJAMBI_TRY {
        QJSEngine *__qt_this = qtjambi_object_from_nativeId<QJSEngine>(__this_nativeId);
        qtjambi_check_resource(env, __qt_this);
        QVariant variant = qtjambi_to_qvariant(env, object);
        CreateManaged createManaged = qjsvalue_cast<CreateManaged>(QJSManagedValue{});
        QJSManagedValue jsval = (__qt_this->*createManaged)(variant.metaType(), variant.data());
        result = qtjambi_cast<jobject>(env, jsval);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}
#endif

