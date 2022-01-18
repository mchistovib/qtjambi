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

package io.qt.widgets;

import java.io.Serializable;
import java.lang.invoke.MethodHandleInfo;
import java.lang.invoke.SerializedLambda;
import java.util.Objects;

import io.qt.core.QByteArray;
import io.qt.core.QMetaObject;
import io.qt.internal.QtJambiInternal;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qstandarditemeditorcreator.html">QStandardItemEditorCreator</a></p>
 */
public class QStandardItemEditorCreator<T extends QWidget> implements QItemEditorCreatorBase {
	
	public interface ConstructorHandle<T> extends QItemEditorCreator.ConstructorHandle<T>, Serializable{
	}
	
	private final QByteArray valuePropertyName;
	private final ConstructorHandle<T> constructorHandle;
	
    public QStandardItemEditorCreator(ConstructorHandle<T> constructor) {
        super();
        SerializedLambda serializedLambda = QtJambiInternal.serializeLambdaExpression(Objects.requireNonNull(constructor));
        if(serializedLambda==null || serializedLambda.getImplMethodKind()!=MethodHandleInfo.REF_newInvokeSpecial)
        	throw new RuntimeException("Constructor handle required as argument, e.g. QWidget::new");
        Class<?> implClass;
        try {
        	implClass = constructor.getClass().getClassLoader().loadClass(serializedLambda.getImplClass().replace('/', '.'));
		} catch (ClassNotFoundException e) {
			throw new RuntimeException(e);
		}
		this.valuePropertyName = new QByteArray(QMetaObject.forType(implClass).userProperty().name());
        this.constructorHandle = constructor;
    }
    
    public QStandardItemEditorCreator(Class<T> widgetType) {
        this(widgetType, QItemEditorCreator.findConstructor("QStandardItemEditorCreator", widgetType));
    }
    
    private QStandardItemEditorCreator(Class<T> widgetType, long constructorHandle) {
    	super();
        this.constructorHandle = parent -> QItemEditorCreator.construct(widgetType, constructorHandle, parent);
        this.valuePropertyName = new QByteArray(QMetaObject.forType(widgetType).userProperty().name());
    }

    @Override
    public T createWidget(QWidget parent) {
    	T widget = this.constructorHandle.create(parent);
    	if(widget!=null && widget.parent()==null) {
    		QtJambi_LibraryUtilities.internal.setJavaOwnership(widget);
    	}
    	return widget;
    }

    @Override
    public QByteArray valuePropertyName() {
        return valuePropertyName.clone();
    }
}
