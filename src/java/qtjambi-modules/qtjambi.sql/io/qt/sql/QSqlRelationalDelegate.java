/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

package io.qt.sql;

import io.qt.core.QAbstractItemModel;
import io.qt.core.QModelIndex;
import io.qt.core.QObject;
import io.qt.core.Qt;
import io.qt.widgets.QComboBox;
import io.qt.widgets.QItemDelegate;
import io.qt.widgets.QStyleOptionViewItem;
import io.qt.widgets.QWidget;

public class QSqlRelationalDelegate extends QItemDelegate {
	
	static {
        QtJambi_LibraryUtilities.initialize();
    }
	
	public QSqlRelationalDelegate() {
		super((QObject)null);
    }

    public QSqlRelationalDelegate(QObject parent) {
        super(parent);
    }

	protected QSqlRelationalDelegate(QDeclarativeConstructor constructor) {
		super(constructor);
	}

	protected QSqlRelationalDelegate(QPrivateConstructor p) {
		super(p);
	}


	@Override
    public QWidget createEditor(QWidget parent,
                                QStyleOptionViewItem option,
                                QModelIndex index) {
        QSqlRelationalTableModel sqlModel = (QSqlRelationalTableModel)index.model();
        QSqlTableModel childModel = sqlModel != null ? sqlModel.relationModel(index.column()) : null;
        if (childModel == null)
            return super.createEditor(parent, option, index);

        QComboBox combo = new QComboBox(parent);
        combo.setModel(childModel);
        combo.setModelColumn(childModel.fieldIndex(sqlModel.relation(index.column()).displayColumn()));
        combo.installEventFilter(this);

        return combo;
    }

    @Override
    public void setEditorData(QWidget editor, QModelIndex index) {
        QSqlRelationalTableModel sqlModel = (QSqlRelationalTableModel)index.model();
        QComboBox combo = editor instanceof QComboBox ? (QComboBox) editor : null;
        if (sqlModel == null || combo == null) {
            super.setEditorData(editor, index);
            return;
        }
        combo.setCurrentIndex(combo.findText(sqlModel.data(index).toString()));
    }

    @Override
    public void setModelData(QWidget editor, QAbstractItemModel model, QModelIndex index) {
        if (index == null)
            return;

        QSqlRelationalTableModel sqlModel = (QSqlRelationalTableModel)model;
        QSqlTableModel childModel = sqlModel != null ? sqlModel.relationModel(index.column()) : null;
        QComboBox combo = editor instanceof QComboBox ? (QComboBox) editor : null;
        if (sqlModel == null || childModel == null || combo == null) {
            super.setModelData(editor, model, index);
            return;
        }

        int currentItem = combo.currentIndex();
        int childColIndex = childModel.fieldIndex(sqlModel.relation(index.column()).displayColumn());
        int childEditIndex = childModel.fieldIndex(sqlModel.relation(index.column()).indexColumn());
        sqlModel.setData(index,
                         childModel.data(childModel.index(currentItem, childColIndex), Qt.ItemDataRole.DisplayRole),
                         Qt.ItemDataRole.DisplayRole);
        sqlModel.setData(index,
                childModel.data(childModel.index(currentItem, childEditIndex), Qt.ItemDataRole.EditRole),
                Qt.ItemDataRole.EditRole);
    }
}
