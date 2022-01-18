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

#include "qtjambi_core.h"
#include "qtjambi_repository_p.h"
#include "qtjambi_registry_p.h"
#include <QtCore/QMutex>

typedef QList<void**> GlobalClassPointers;
Q_GLOBAL_STATIC(GlobalClassPointers, gGlobalClassPointers)
Q_GLOBAL_STATIC(QRecursiveMutex, gMutex)

JNIEnv *qtjambi_current_environment(bool initializeJavaThread);

void clear_repository_at_shutdown(JNIEnv *){
    GlobalClassPointers globalClassPointers;
    {
        QRecursiveMutexLocker locker(gMutex());
        Q_UNUSED(locker)
        globalClassPointers.swap(*gGlobalClassPointers);
    }
    for(void** ptr : globalClassPointers){
        *ptr = nullptr;
    }
}

#ifdef POST_CLASS_REF_ACTION
#undef POST_CLASS_REF_ACTION
#endif
#define POST_CLASS_REF_ACTION(class_ref) \
{\
    QRecursiveMutexLocker locker(gMutex());\
    Q_UNUSED(locker)\
    gGlobalClassPointers->append(reinterpret_cast<void**>(&class_ref));\
}

#define DEFINE_CLASS_REF(cls)\
    this->__##cls = getGlobalClassRef(env, jclass(env->GetStaticObjectField(this->class_ref, env->GetStaticFieldID(this->class_ref, #cls, "Ljava/lang/Class;") )), nullptr);\
    POST_CLASS_REF_ACTION(this->__##cls)\
    qtjambi_throw_java_exception(env);

namespace Java{

namespace QtCore{

namespace Internal{
        QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QCoreApplication,
                                 QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(execPreRoutines,()V)
                                 QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(execPostRoutines,()V))
}

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QThread,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(javaThread,Ljava/lang/Thread;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_FIELD(interruptible,Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMetaMethod,
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QDeclarativeConstructor,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Class;J)
    QTJAMBI_REPOSITORY_DEFINE_LONG_FIELD(placement)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(cls,Ljava/lang/Class;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QBindable,
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(createBinding,(Lio/qt/core/QUntypedPropertyData;)Lio/qt/core/QUntypedBindable;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QProperty,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()Ljava/lang/Object;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(Ljava/lang/Object;)V)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QPropertyBinding;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QPropertyBinding;)Lio/qt/core/QPropertyBinding;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QPropertyBinding;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(hasSignal,()Z)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(emitSignal,()V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QBooleanProperty,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()Z)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(Z)V)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QBooleanPropertyBinding;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QBooleanPropertyBinding;)Lio/qt/core/QBooleanPropertyBinding;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QBooleanPropertyBinding;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(hasSignal,()Z)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(emitSignal,()V)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QByteProperty,
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()B)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(B)V)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QBytePropertyBinding;)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QBytePropertyBinding;)Lio/qt/core/QBytePropertyBinding;)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QBytePropertyBinding;)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(hasSignal,()Z)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(emitSignal,()V)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QShortProperty,
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()S)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(S)V)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QShortPropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QShortPropertyBinding;)Lio/qt/core/QShortPropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QShortPropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(hasSignal,()Z)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(emitSignal,()V)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QCharProperty,
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()C)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(C)V)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QCharPropertyBinding;)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QCharPropertyBinding;)Lio/qt/core/QCharPropertyBinding;)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QCharPropertyBinding;)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(hasSignal,()Z)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(emitSignal,()V)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QIntProperty,
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()I)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(I)V)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QIntPropertyBinding;)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QIntPropertyBinding;)Lio/qt/core/QIntPropertyBinding;)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QIntPropertyBinding;)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(hasSignal,()Z)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(emitSignal,()V)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QLongProperty,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()J)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(J)V)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QLongPropertyBinding;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QLongPropertyBinding;)Lio/qt/core/QLongPropertyBinding;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QLongPropertyBinding;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(hasSignal,()Z)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(emitSignal,()V)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QDoubleProperty,
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()D)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(D)V)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QDoublePropertyBinding;)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QDoublePropertyBinding;)Lio/qt/core/QDoublePropertyBinding;)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QDoublePropertyBinding;)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(hasSignal,()Z)
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(emitSignal,()V)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QFloatProperty,
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()F)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(F)V)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QFloatPropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QFloatPropertyBinding;)Lio/qt/core/QFloatPropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QFloatPropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(hasSignal,()Z)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(emitSignal,()V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QAbstractComputedProperty,
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QComputedProperty,
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()Ljava/lang/Object;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QComputedBooleanProperty,
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()Z)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QComputedByteProperty,
                           QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()B)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QComputedShortProperty,
                            QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()S)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QComputedCharProperty,
                           QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()C)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QComputedIntProperty,
                             QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()I)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QComputedLongProperty,
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()J)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QComputedDoubleProperty,
                           QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()D)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QComputedFloatProperty,
                            QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()F)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QCoreApplication,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_FIELD(__qt_isInitializing,Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMetaObject$Slot1,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(invoke,(Ljava/lang/Object;)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMetaObject$Slot0,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(invoke,()V)
)

}

namespace Runtime{

QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(java/lang,String)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/concurrent/atomic,AtomicBoolean,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(set,(Z)V)
)


QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/concurrent/atomic,AtomicInteger,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(set,(I)V)
)


QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/concurrent/atomic,AtomicLong,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(J)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,()J)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(set,(J)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/concurrent/atomic,AtomicReference,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,()Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(set,(Ljava/lang/Object;)V)
)


QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Number,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(intValue,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(doubleValue,()D)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(longValue,()J)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(floatValue,()F)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(shortValue,()S)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(byteValue,()B)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Boolean,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(valueOf,(Z)Ljava/lang/Boolean;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(booleanValue,()Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(getBoolean,(Ljava/lang/String;)Z)
    DEFINE_CLASS_REF(TYPE)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Integer,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(valueOf,(I)Ljava/lang/Integer;)
    DEFINE_CLASS_REF(TYPE)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Void,
    DEFINE_CLASS_REF(TYPE)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Double,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(valueOf,(D)Ljava/lang/Double;)
    DEFINE_CLASS_REF(TYPE)
)


QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Long,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(valueOf,(J)Ljava/lang/Long;)
    DEFINE_CLASS_REF(TYPE)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Float,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(valueOf,(F)Ljava/lang/Float;)
    DEFINE_CLASS_REF(TYPE)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Short,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(valueOf,(S)Ljava/lang/Short;)
    DEFINE_CLASS_REF(TYPE)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Byte,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(valueOf,(B)Ljava/lang/Byte;)
    DEFINE_CLASS_REF(TYPE)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Character,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(charValue,()C)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(valueOf,(C)Ljava/lang/Character;)
    DEFINE_CLASS_REF(TYPE)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/nio,ByteBuffer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asCharBuffer,()Ljava/nio/CharBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asDoubleBuffer,()Ljava/nio/DoubleBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asFloatBuffer,()Ljava/nio/FloatBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asIntBuffer,()Ljava/nio/IntBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asLongBuffer,()Ljava/nio/LongBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asShortBuffer,()Ljava/nio/ShortBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asReadOnlyBuffer,()Ljava/nio/ByteBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,(I)B)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(put,(IB)Ljava/nio/ByteBuffer;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/nio,IntBuffer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asReadOnlyBuffer,()Ljava/nio/IntBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,(I)I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(put,(II)Ljava/nio/IntBuffer;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/nio,LongBuffer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asReadOnlyBuffer,()Ljava/nio/LongBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,(I)J)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(put,(IJ)Ljava/nio/LongBuffer;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/nio,ShortBuffer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asReadOnlyBuffer,()Ljava/nio/ShortBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,(I)S)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(put,(IS)Ljava/nio/ShortBuffer;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/nio,FloatBuffer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asReadOnlyBuffer,()Ljava/nio/FloatBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,(I)F)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(put,(IF)Ljava/nio/FloatBuffer;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/nio,DoubleBuffer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asReadOnlyBuffer,()Ljava/nio/DoubleBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,(I)D)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(put,(ID)Ljava/nio/DoubleBuffer;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/nio,CharBuffer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asReadOnlyBuffer,()Ljava/nio/CharBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,(I)C)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(put,(IC)Ljava/nio/CharBuffer;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/nio,Buffer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isDirect,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isReadOnly,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(capacity,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(limit,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(position,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(mark,()Ljava/nio/Buffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(array,()Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(hasArray,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(arrayOffset,()I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Runnable,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(run,()V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,Supplier,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,()Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,DoubleSupplier,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getAsDouble,()D)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,IntSupplier,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getAsInt,()I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,LongSupplier,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getAsLong,()J)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,BooleanSupplier,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getAsBoolean,()Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,Consumer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(accept,(Ljava/lang/Object;)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,DoubleConsumer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(accept,(D)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,IntConsumer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(accept,(I)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,LongConsumer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(accept,(J)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,ObjDoubleConsumer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(accept,(Ljava/lang/Object;D)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,ObjIntConsumer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(accept,(Ljava/lang/Object;I)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,ObjLongConsumer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(accept,(Ljava/lang/Object;J)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,BiConsumer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(accept,(Ljava/lang/Object;Ljava/lang/Object;)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,Function,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(apply,(Ljava/lang/Object;)Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,IntFunction,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(apply,(I)Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,DoubleFunction,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(apply,(D)Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,LongFunction,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(apply,(J)Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,ToIntFunction,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(applyAsInt,(Ljava/lang/Object;)I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,DoubleToIntFunction,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(applyAsInt,(D)I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,LongToIntFunction,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(applyAsInt,(J)I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,ToIntBiFunction,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(applyAsInt,(Ljava/lang/Object;Ljava/lang/Object;)I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,IntToDoubleFunction,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(applyAsDouble,(I)D)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,LongToDoubleFunction,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(applyAsDouble,(J)D)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,ToDoubleFunction,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(applyAsDouble,(Ljava/lang/Object;)D)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,ToDoubleBiFunction,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(applyAsDouble,(Ljava/lang/Object;Ljava/lang/Object;)D)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,ToLongFunction,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(applyAsLong,(Ljava/lang/Object;)J)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,DoubleToLongFunction,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(applyAsLong,(D)J)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,IntToLongFunction,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(applyAsLong,(I)J)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,ToLongBiFunction,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(applyAsLong,(Ljava/lang/Object;Ljava/lang/Object;)J)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,BiFunction,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(apply,(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,BiPredicate,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(test,(Ljava/lang/Object;Ljava/lang/Object;)Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,Predicate,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(test,(Ljava/lang/Object;)Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,DoublePredicate,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(test,(D)Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,IntPredicate,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(test,(I)Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,LongPredicate,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(test,(J)Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,IntUnaryOperator,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(applyAsInt,(I)I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,DoubleUnaryOperator,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(applyAsDouble,(D)D)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,LongUnaryOperator,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(applyAsLong,(J)J)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,IntBinaryOperator,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(applyAsInt,(II)I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,DoubleBinaryOperator,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(applyAsDouble,(DD)D)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/function,LongBinaryOperator,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(applyAsLong,(JJ)J)
)

}

namespace QtGui{

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/gui,QValidator$QValidationData,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;I)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(string,Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(position,I)
)

}

namespace QtCore{

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QtMessageHandler,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(accept,(Lio/qt/core/QtMsgType;Lio/qt/core/QMessageLogContext;Ljava/lang/String;)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QPair,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(first,Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(second,Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QModelIndex,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(IIJLio/qt/core/QAbstractItemModel;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(row,I)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(column,I)
    QTJAMBI_REPOSITORY_DEFINE_LONG_FIELD(internalId)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(model,Lio/qt/core/QAbstractItemModel;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMetaObject,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(J)
    QTJAMBI_REPOSITORY_DEFINE_LONG_FIELD(metaObjectPointer)
)

QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(io/qt/core,QMetaObject$Connection)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal/fileengine,QClassPathEngine,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMetaType$GenericTypeInterface,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(metaType,()Lio/qt/core/QMetaType;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMetaEnum,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(flagsType,(Ljava/lang/Class;)Ljava/lang/Class;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMetaType$GenericValue,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(type,I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMetaType$GenericObject,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(type,I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMetaType$GenericGadget,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(type,I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMetaType$GenericEnumerator,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(IIILjava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMetaType$GenericByteEnumerator,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(IIBLjava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMetaType$GenericShortEnumerator,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(IISLjava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMetaType$GenericLongEnumerator,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(IIJLjava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMetaType$GenericFlags,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(II)
)

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QLinkedList,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/QtObject$QPrivateConstructor;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QVector,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/QtObject$QPrivateConstructor;)
)
#endif

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QStringList,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/QtObject$QPrivateConstructor;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QQueue,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/QtObject$QPrivateConstructor;)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/util/Collection;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QList,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/QtObject$QPrivateConstructor;)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/util/Collection;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QStack,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/QtObject$QPrivateConstructor;)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/util/Collection;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMap,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/QtObject$QPrivateConstructor;)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/util/Map;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QHash,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/QtObject$QPrivateConstructor;)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/util/Map;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMultiMap,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/QtObject$QPrivateConstructor;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMultiHash,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/QtObject$QPrivateConstructor;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QSet,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/QtObject$QPrivateConstructor;)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/util/Collection;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QIterator,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMapIterator,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(io/qt/core,QByteArray)

QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(io/qt/core,QObject)

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFutureWatcher,
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(JJJ)
        QTJAMBI_REPOSITORY_DEFINE_METHOD(future,()Lio/qt/core/QFuture;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFuture,
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QFutureInterfaceBase;Z)
        QTJAMBI_REPOSITORY_DEFINE_FIELD(d,Lio/qt/core/QFutureInterfaceBase;)
    )
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QPromise,
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QFutureInterfaceBase;Z)
        QTJAMBI_REPOSITORY_DEFINE_FIELD(d,Lio/qt/core/QFutureInterfaceBase;)
        QTJAMBI_REPOSITORY_DEFINE_FIELD(nativeInstance,Lio/qt/core/QPromise$NativeInstance;)
    )
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QPromise$NativeInstance,
        QTJAMBI_REPOSITORY_DEFINE_FIELD(promise,Lio/qt/core/QPromise;)
    )
#endif

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFutureInterface,
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/QtObject$QPrivateConstructor;)
    )
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QGenericArgumentType,
                                     QTJAMBI_REPOSITORY_DEFINE_FIELD(metaType,Lio/qt/core/QMetaType;)
                                     QTJAMBI_REPOSITORY_DEFINE_FIELD(classType,Ljava/lang/Class;)
                                     QTJAMBI_REPOSITORY_DEFINE_FIELD(pointerOrReference,I))
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QGenericArgument,
                                     QTJAMBI_REPOSITORY_DEFINE_FIELD(value,Ljava/lang/Object;)
                                     QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(metaType,(Ljava/lang/Object;)Lio/qt/core/QMetaType;))
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFunctionPointer,)
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFunctionPointerUtil,
                                     QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(createProxy,(Ljava/lang/Class;)Lio/qt/QtObjectInterface;)
                                     QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(createCallbackProxy,(Ljava/lang/Class;Ljava/lang/Class;Lio/qt/QtObjectInterface;)Lio/qt/core/QFunctionPointerUtil$CppToJavaInvocationHandler;)
                                     QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(registerCleanup,(Lio/qt/QtObjectInterface;Lio/qt/core/QFunctionPointerUtil$CppToJavaInvocationHandler;)V))
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFunctionPointerUtil$CppToJavaInvocationHandler,
                                     QTJAMBI_REPOSITORY_DEFINE_FIELD(proxy,Ljava/lang/Object;)
                                     QTJAMBI_REPOSITORY_DEFINE_FIELD(peer,J))

     QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QVariant,
         QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(ILjava/lang/Object;Z)
         QTJAMBI_REPOSITORY_DEFINE_METHOD(userType,()I)
         QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()Ljava/lang/Object;)
     )
}

namespace Runtime {

namespace Internal{
QTJAMBI_REPOSITORY_DEFINE_CLASS(java/nio,ByteBuffer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(allocateDirect,(I)Ljava/nio/ByteBuffer;)
)
}

QTJAMBI_REPOSITORY_DEFINE_CLASS_SC(java/util,HashSet)
QTJAMBI_REPOSITORY_DEFINE_CLASS_SC(java/util,ArrayList)
QTJAMBI_REPOSITORY_DEFINE_CLASS_SC(java/util,ArrayDeque)
QTJAMBI_REPOSITORY_DEFINE_CLASS_SC(java/util,LinkedList)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang/invoke,MethodHandles$Lookup,)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util,List,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,(I)Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(set,(ILjava/lang/Object;)Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_RENAMED_METHOD(removeByIndex,remove,(I)Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(remove,(Ljava/lang/Object;)Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util,Set,
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util,Map,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(put,(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(putAll,(Ljava/util/Map;)V)
    QTJAMBI_REPOSITORY_DEFINE_RENAMED_METHOD(removePair,remove,(Ljava/lang/Object;Ljava/lang/Object;)Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(remove,(Ljava/lang/Object;)Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(size,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(clear,()V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(entrySet,()Ljava/util/Set;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,(Ljava/lang/Object;)Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util,Map$Entry,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getKey,()Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getValue,()Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Comparable,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(compareTo,(Ljava/lang/Object;)I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util,HashMap,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util,NavigableMap,
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util,Deque,
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util,Queue,
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util,TreeMap,
    QTJAMBI_REPOSITORY_DEFINE_STANDARD_CONSTRUCTOR()
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/util/Comparator;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util,TreeSet,
    QTJAMBI_REPOSITORY_DEFINE_STANDARD_CONSTRUCTOR()
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/util/Comparator;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,NullPointerException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,IllegalAccessError,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Error,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,RuntimeException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,UnsupportedOperationException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,ClassCastException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,IndexOutOfBoundsException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util,Collection,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(add,(Ljava/lang/Object;)Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(addAll,(Ljava/util/Collection;)Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(size,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(toArray,()[Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(clear,()V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(remove,(Ljava/lang/Object;)Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(contains,(Ljava/lang/Object;)Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(iterator,()Ljava/util/Iterator;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util,Iterator,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(remove,()V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(hasNext,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(next,()Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util,Arrays,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(copyOf,([Ljava/lang/Object;I)[Ljava/lang/Object;)
)

    QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util,Objects,
        QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(equals,(Ljava/lang/Object;Ljava/lang/Object;)Z)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang/reflect,Constructor,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(getDeclaringClass,()Ljava/lang/Class;)
        QTJAMBI_REPOSITORY_DEFINE_METHOD(getName,()Ljava/lang/String;)
    )

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang/reflect,Method,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getDeclaringClass,()Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getName,()Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getReturnType,()Ljava/lang/Class;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang/reflect,Executable,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getModifiers,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getParameterTypes,()[Ljava/lang/Class;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang/reflect,Field,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getDeclaringClass,()Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getModifiers,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getName,()Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getType,()Ljava/lang/Class;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang/reflect,Modifier,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(isNative,(I)Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(isStatic,(I)Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(isFinal,(I)Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(isAbstract,(I)Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Object,
    QTJAMBI_REPOSITORY_DEFINE_STANDARD_CONSTRUCTOR()
    QTJAMBI_REPOSITORY_DEFINE_METHOD(equals,(Ljava/lang/Object;)Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(toString,()Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(hashCode,()I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Class,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getName,()Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(cast,(Ljava/lang/Object;)Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getDeclaredMethods,()[Ljava/lang/reflect/Method;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getDeclaredField,(Ljava/lang/String;)Ljava/lang/reflect/Field;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getEnumConstants,()[Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getClassLoader,()Ljava/lang/ClassLoader;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isAnnotationPresent,(Ljava/lang/Class;)Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getAnnotation,(Ljava/lang/Class;)Ljava/lang/annotation/Annotation;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getInterfaces,()[Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isInterface,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getModifiers,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getComponentType,()Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getTypeParameters,()[Ljava/lang/reflect/TypeVariable;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isArray,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isPrimitive,()Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util,Optional,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(empty,()Ljava/util/Optional;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,()Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isPresent,()Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(ofNullable,(Ljava/lang/Object;)Ljava/util/Optional;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util,OptionalInt,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(empty,()Ljava/util/OptionalInt;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getAsInt,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isPresent,()Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(of,(I)Ljava/util/OptionalInt;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util,OptionalLong,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(empty,()Ljava/util/OptionalLong;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getAsLong,()J)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isPresent,()Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(of,(J)Ljava/util/OptionalLong;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util,OptionalDouble,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(empty,()Ljava/util/OptionalDouble;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getAsDouble,()D)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isPresent,()Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(of,(D)Ljava/util/OptionalDouble;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,System,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(gc,()V)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(getProperty,(Ljava/lang/String;)Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(identityHashCode,(Ljava/lang/Object;)I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/net,URL,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/net,URLClassLoader,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(newInstance,([Ljava/net/URL;Ljava/lang/ClassLoader;)Ljava/net/URLClassLoader;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(addURL,(Ljava/net/URL;)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,ClassLoader,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(loadClass,(Ljava/lang/String;)Ljava/lang/Class;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,NumberFormatException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,IllegalArgumentException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,IllegalStateException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,StackTraceElement,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getClassName,()Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getMethodName,()Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Thread,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(currentThread,()Ljava/lang/Thread;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getUncaughtExceptionHandler,()Ljava/lang/Thread$UncaughtExceptionHandler;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getContextClassLoader,()Ljava/lang/ClassLoader;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(setUncaughtExceptionHandler,(Ljava/lang/Thread$UncaughtExceptionHandler;)V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(setContextClassLoader,(Ljava/lang/ClassLoader;)V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getThreadGroup,()Ljava/lang/ThreadGroup;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getStackTrace,()[Ljava/lang/StackTraceElement;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(interrupt,()V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isAlive,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isDaemon,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getName,()Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,ThreadGroup,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(add,(Ljava/lang/Thread;)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Throwable,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(addSuppressed,(Ljava/lang/Throwable;)V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getMessage,()Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Enum,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(ordinal,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(name,()Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang/reflect,AccessibleObject,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isAnnotationPresent,(Ljava/lang/Class;)Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getAnnotation,(Ljava/lang/Class;)Ljava/lang/annotation/Annotation;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,IllegalAccessException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

}

namespace QtGui{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/gui,QPainter,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(beginPaint,(Lio/qt/gui/QPaintDevice;)V)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(endPaint,(Lio/qt/gui/QPaintDevice;)V)
)
}

namespace QtJambi{

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,QtJambiInternal$NativeLink,
    QTJAMBI_REPOSITORY_DEFINE_LONG_FIELD(native__id)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(detach,(JZ)V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(reset,()V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,QtJambiInternal,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(findQmlAttachedProperties,(Ljava/lang/Class;)Ljava/lang/reflect/Method;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(getImplementedInterfaces,(Ljava/lang/Class;)Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(getAllImplementedInterfaces,(Ljava/lang/Class;)Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(checkImplementation,(Ljava/lang/Class;Ljava/lang/Class;)V)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(isImplementedInJava,(ZLjava/lang/reflect/Method;Ljava/lang/Class;)Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(findGeneratedSuperclass,(Ljava/lang/Class;)Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(writeSerializableJavaObject,(Lio/qt/core/QDataStream;Ljava/lang/Object;)V)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(debugObject,(Lio/qt/core/QDebug;Ljava/lang/Object;)V)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(readSerializableJavaObject,(Lio/qt/core/QDataStream;)Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(isGeneratedClass,(Ljava/lang/Class;)Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(putMultiMap,(Ljava/util/Map;Ljava/lang/Object;Ljava/lang/Object;)Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(createComparator,(J)Ljava/util/Comparator;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(getLibraryPaths,()Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(reportException,(Ljava/lang/String;Ljava/lang/Throwable;)V)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(extendStackTrace,(Ljava/lang/Throwable;Ljava/lang/String;Ljava/lang/String;I)V)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(findInterfaceLink,(Lio/qt/QtObjectInterface;ZZ)Lio/qt/internal/QtJambiInternal$NativeLink;)
    QTJAMBI_REPOSITORY_DEFINE_RENAMED_STATIC_METHOD(createNativeLinkInterface,createNativeLink,(Lio/qt/QtObjectInterface;)Lio/qt/internal/QtJambiInternal$NativeLink;)
    QTJAMBI_REPOSITORY_DEFINE_RENAMED_STATIC_METHOD(createNativeLinkObject,createNativeLink,(Lio/qt/internal/QtJambiObject;)Lio/qt/internal/QtJambiInternal$NativeLink;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(setThreadInterruptible,(Ljava/lang/Thread;Ljava/lang/Object;)Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(createAssociation,(Ljava/lang/Object;Ljava/lang/Object;)V)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(deleteAssociation,(Ljava/lang/Object;)Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(findAssociation,(Ljava/lang/Object;)Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(objectToString,(Ljava/lang/Object;)Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(lambdaReturnType,(Ljava/io/Serializable;)Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(terminateCleanupThread,()V)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_FIELD(internalAccess,Lio/qt/InternalAccess;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,NativeLibraryManager,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(resetDeploymentSpecs,()V)
)

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,QtJambiEnums,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(extendEnum,(Ljava/lang/Class;[Ljava/lang/Enum;Ljava/lang/Enum;)Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,QtJambiPlugins,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(loadPluginInstance,(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Lio/qt/core/QObject;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,QtJambiObject,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(nativeLink,Lio/qt/internal/QtJambiInternal$NativeLink;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS_SC(io/qt/internal,QtJambiInternal$RCList)
QTJAMBI_REPOSITORY_DEFINE_CLASS_SC(io/qt/internal,QtJambiInternal$RCSet)
QTJAMBI_REPOSITORY_DEFINE_CLASS_SC(io/qt/internal,QtJambiInternal$RCMap)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,MetaObjectTools,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(buildMetaData,(Ljava/lang/Class;)Lio/qt/internal/MetaObjectTools$MetaData;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(getEnumForQFlags,(Ljava/lang/Class;)Ljava/lang/Class;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,MetaObjectTools$SignalInfo,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(field,Ljava/lang/reflect/Field;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(signalTypes,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(signalClass,Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(signalMetaTypes,[I)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(methodId,J)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,MetaObjectTools$MetaData,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(metaData,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(stringData,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(signalInfos,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(methods,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(methodMetaTypes,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(constructors,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(constructorMetaTypes,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(propertyReaders,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(propertyWriters,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(propertyResetters,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(propertyNotifies,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(propertyMemberFields,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD_QT5(propertyDesignableResolvers,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD_QT5(propertyScriptableResolvers,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD_QT5(propertyEditableResolvers,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD_QT5(propertyStoredResolvers,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD_QT5(propertyUserResolvers,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD_QT6(propertyQPropertyFields,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD_QT6(propertyBindables,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(propertyMetaTypes,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(propertyClassTypes,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD_QT6(metaTypes,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(relatedMetaObjects,Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(hasStaticMembers,Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QThreadAffinityException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;Lio/qt/core/QObject;Lio/qt/core/QThread;Lio/qt/core/QThread;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QInterfaceCannotBeSubclassedException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Class;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QFlags,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(I)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QFlags$ConcreteWrapper,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtEnumerator,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()I)
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(comparator,()Ljava/util/Comparator;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtFlagEnumerator,
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtByteEnumerator,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()B)
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(comparator,()Ljava/util/Comparator;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtShortEnumerator,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()S)
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(comparator,()Ljava/util/Comparator;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtLongEnumerator,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()J)
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(comparator,()Ljava/util/Comparator;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtGadget,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(staticMetaObject,Lio/qt/core/QMetaObject;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,QtJambiSignals$AbstractSignal,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(initializeSignal,(Ljava/lang/Class;Ljava/util/List;IJ)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,QtJambiSignals$SignalInfo,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(ILjava/util/List;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,QtJambiSignals$SignalParameterType,
                                QTJAMBI_REPOSITORY_DEFINE_FIELD(type,Ljava/lang/Class;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QMissingVirtualOverridingException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QUnsuccessfulInvocationException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtRejectedEntries,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()[Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(io/qt,QtExtensibleEnum)

QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(io/qt,QtPrivateOverride)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,QtJambiSignals$AbstractConnection,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(invoke,([Ljava/lang/Object;)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,QtJambiSignals$NativeConnection,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/QtObject$QPrivateConstructor;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,QtJambiSignals,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(resolveSignal,(Ljava/lang/Object;[Z[Z)Ljava/util/List;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,QtJambiSignals$AbstractMultiSignal,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(initializeSignals,(Ljava/lang/reflect/Field;[I[J[Ljava/util/List;[Ljava/lang/Class;)V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(signal,(I)Lio/qt/internal/QtJambiSignals$AbstractSignal;)
)

QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(io/qt,QtObject)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtObjectInterface,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(dispose,()V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isDisposed,()Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtArgument$Stream$Arg,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(type,Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(value,Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QNativePointer,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(fromNative,(JIJIZ)Lio/qt/QNativePointer;)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(IJIZ)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/nio/Buffer;J)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(indirections,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(byteSize,()J)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isReadOnly,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(pointer,()J)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QInstanceMemberSignals$Signal0,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QInstanceMemberSignals$Signal1,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QInstanceMemberSignals$Signal2,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QInstanceMemberSignals$Signal3,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QInstanceMemberSignals$Signal4,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QInstanceMemberSignals$Signal5,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QInstanceMemberSignals$Signal6,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QInstanceMemberSignals$Signal7,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QInstanceMemberSignals$Signal8,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QInstanceMemberSignals$Signal9,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QNoNativeResourcesException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QNonVirtualOverridingException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QNoImplementationException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QMisfittingSignatureException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

}

namespace JNA{
QTJAMBI_REPOSITORY_DEFINE_CLASS(com/sun/jna,Native,
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(setProtected,(Z)V)
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(invokeStructure,(Lcom/sun/jna/Function;JI[Ljava/lang/Object;JJ)V)
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(createNativeCallback,(Lcom/sun/jna/Callback;Ljava/lang/reflect/Method;[Ljava/lang/Class;Ljava/lang/Class;IILjava/lang/String;)J)
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(freeNativeCallback,(J)V)
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(ffi_prep_cif,(IIJJ)J)
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(ffi_call,(JJJJ)V)
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(ffi_prep_closure,(JLcom/sun/jna/Native$ffi_callback;)J)
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(ffi_free_closure,(J)V)
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(free,(J)V)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(com/sun/jna,Function,
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(getFunction,(Lcom/sun/jna/Pointer;)Lcom/sun/jna/Function;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(invoke,(Ljava/lang/Class;[Ljava/lang/Object;)Ljava/lang/Object;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(com/sun/jna,Pointer,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(J)
                                QTJAMBI_REPOSITORY_DEFINE_FIELD(peer,J)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(com/sun/jna,Structure,
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(com/sun/jna,CallbackProxy,
                                QTJAMBI_REPOSITORY_DEFINE_RENAMED_METHOD(callbackMethod,callback,([Ljava/lang/Object;)Ljava/lang/Object;)
QTJAMBI_REPOSITORY_DEFINE_METHOD(getParameterTypes,()[Ljava/lang/Class;)
QTJAMBI_REPOSITORY_DEFINE_METHOD(getReturnType,()Ljava/lang/Class;)
)
}

}

