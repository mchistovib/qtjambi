/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.autotests;

import static org.junit.Assert.assertEquals;
import org.junit.Test;

import io.qt.autotests.generated.SqlTableModelSubclass;
import io.qt.core.QMetaType;
import io.qt.core.QVariant;
import io.qt.sql.QSqlDatabase;
import io.qt.sql.QSqlField;
import io.qt.sql.QSqlRecord;

// SQL support is an optional part of API:
// 1) The javac has to compile this package (this is the usual way the
//    test is deselected by having javac just not compile it)
// 2) The ANT testrunner looks over the source code folder for tests the
//    problem is that this class won't load in environment where OpenGL
//    package does not exist.  FIXME
public class TestSqlInjectedCodeQt6 extends ApplicationInitializer {

    // Used by testSqlTableModelBeforeInsertJava()
	private void receiveBeforeInsert(QSqlRecord record) {
        record.append(new QSqlField("javaInt", new QMetaType(QMetaType.Type.Int)));
        record.setValue("javaInt", 3456);
    }

    @Test
    public void testSqlTableModelBeforeInsertJava() {
        SqlTableModelSubclass stms = new SqlTableModelSubclass();

        stms.beforeInsert.connect(this::receiveBeforeInsert);
        stms.emitBeforeInsert();

        QSqlRecord record = stms.myRecord();
        assertEquals(3456, record.value("javaInt"));
        assertEquals(3456, QVariant.toInt(record.value("javaInt")));
    }

    @Test
    public void testSqlTableModelBeforeInsertCpp() {
        SqlTableModelSubclass stms = new SqlTableModelSubclass();

        stms.connectBeforeInsert();

        QSqlRecord record = new QSqlRecord();
        stms.beforeInsert.emit(record);

        assertEquals(1234, QVariant.toInt(record.value("cppInt")));
    }

    @Test
    public void testSqlDatabaseDefaultConnection() {
        String defaultConnection = QSqlDatabase.defaultConnection();
        assertEquals("qt_sql_default_connection", defaultConnection);
    }
}
