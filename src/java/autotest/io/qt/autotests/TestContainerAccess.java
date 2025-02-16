package io.qt.autotests;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

import org.junit.Assert;
import org.junit.Test;

import io.qt.autotests.generated.ContainerTest;
import io.qt.core.QByteArray;
import io.qt.core.QDataStream;
import io.qt.core.QHash;
import io.qt.core.QIODevice;
import io.qt.core.QList;
import io.qt.core.QMap;
import io.qt.core.QMetaType;
import io.qt.core.QMultiHash;
import io.qt.core.QMultiMap;
import io.qt.core.QPair;
import io.qt.core.QPoint;
import io.qt.core.QSet;
import io.qt.internal.QtJambiInternal;

public class TestContainerAccess extends ApplicationInitializer {
    @Test
    public void testQListFloatDoublePair() {
    	QList<QPair<Float,Double>> container = QList.of(new QPair<>(1.0f, 1.1), new QPair<>(2.0f, 2.2), new QPair<>(3.0f, 3.3));
    	List<QPair<Float,Double>> javaContainer = new ArrayList<>();
    	ContainerTest.copyQListFDToJavaList(container, javaContainer);
    	Assert.assertEquals(container, javaContainer);
    	long hash = ContainerTest.getQListFDHash(container);
    	Assert.assertEquals(hash, QtJambiInternal.objectHash(container));
    	QByteArray array = new QByteArray();
    	QDataStream s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	ContainerTest.writeQListFD(s, container);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	QList<QPair<Float,Double>> list2 = new QList<>(QMetaType.fromType(QPair.class, new QMetaType(QMetaType.Type.Float), new QMetaType(QMetaType.Type.Double)));
    	list2.readFrom(s);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	array = new QByteArray();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	container.writeTo(s);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	list2 = new QList<>(QMetaType.fromType(QPair.class, new QMetaType(QMetaType.Type.Float), new QMetaType(QMetaType.Type.Double)));
    	ContainerTest.readQListFD(s, list2);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	
    	javaContainer.clear();
    	ContainerTest.copyFromIterable(list2, javaContainer);
    	Assert.assertEquals(list2, javaContainer);
    	QList<Object> variantList = ContainerTest.toQVariantList(list2);
    	Assert.assertEquals(variantList, container);
    	Assert.assertEquals(container.size(), ContainerTest.containerSize(container));
    }
    
    @Test
    public void testQHashShortDouble() {
    	QHash<Short,Double> container = QHash.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3);
    	Map<Object,Object> javaContainer = new HashMap<>();
    	ContainerTest.copyQHashShortDoubleToJavaList(container, javaContainer);
    	Assert.assertEquals(container, javaContainer);
    	long hash = ContainerTest.getQHashShortDoubleHash(container);
    	Assert.assertEquals(hash, QtJambiInternal.objectHash(container));
    	QByteArray array = new QByteArray();
    	QDataStream s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	ContainerTest.writeQHashShortDouble(s, container);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	QHash<Short,Double> list2 = new QHash<>(short.class, double.class);
    	list2.readFrom(s);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	
    	array = new QByteArray();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	container.writeTo(s);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	list2 = new QHash<>(short.class, double.class);
    	ContainerTest.readQHashShortDouble(s, list2);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	
    	javaContainer.clear();
    	ContainerTest.copyFromIterable(list2, javaContainer);
    	Assert.assertEquals(list2, javaContainer);
    	QHash<String,Object> variantContainer = ContainerTest.toQVariantHash(list2);
    	Map<String,Object> javaStringContainer = new HashMap<>();
    	for(Map.Entry<Object,Object> entry : javaContainer.entrySet()) {
    		javaStringContainer.put(""+entry.getKey(), entry.getValue());
    	}
    	Assert.assertEquals(variantContainer, javaStringContainer);
    	Assert.assertEquals(container.size(), ContainerTest.containerSize(container));
    	Assert.assertTrue(ContainerTest.associativeFind(container, (short)1));
    }
    
    @Test
    public void testQSetQPoint() {
    	QSet<QPoint> container = QSet.of(new QPoint(1, 1), new QPoint(2, 2), new QPoint(3, 3));
    	Set<QPoint> javaContainer = new HashSet<>();
    	ContainerTest.copyQSetQPointToJavaList(container, javaContainer);
    	Assert.assertEquals(container, javaContainer);
    	long hash = ContainerTest.getQSetQPointHash(container);
    	Assert.assertEquals(hash, QtJambiInternal.objectHash(container));
    	QByteArray array = new QByteArray();
    	QDataStream s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	ContainerTest.writeQSetQPoint(s, container);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	QSet<QPoint> list2 = new QSet<>(QPoint.class);
    	list2.readFrom(s);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	
    	array = new QByteArray();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	container.writeTo(s);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	list2 = new QSet<>(QPoint.class);
    	ContainerTest.readQSetQPoint(s, list2);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	
    	javaContainer.clear();
    	ContainerTest.copyFromIterable(list2, javaContainer);
    	Assert.assertEquals(list2, javaContainer);
    	QList<Object> variantList = ContainerTest.toQVariantList(list2);
    	Assert.assertEquals(variantList, container);
    	Assert.assertEquals(container.size(), ContainerTest.containerSize(container));
    }
    
    @Test
    public void testQMapShortDouble() {
    	QMap<Short,Double> container = QMap.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3);
    	Map<Short,Double> javaContainer = new HashMap<>();
    	ContainerTest.copyQMapShortDoubleToJavaList(container, javaContainer);
    	Assert.assertEquals(container, javaContainer);
    	QByteArray array = new QByteArray();
    	QDataStream s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	ContainerTest.writeQMapShortDouble(s, container);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	QMap<Short,Double> list2 = new QMap<>(short.class, double.class);
    	list2.readFrom(s);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	
    	array = new QByteArray();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	container.writeTo(s);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	list2 = new QMap<>(short.class, double.class);
    	ContainerTest.readQMapShortDouble(s, list2);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	
    	javaContainer.clear();
    	ContainerTest.copyFromIterable(list2, javaContainer);
    	Assert.assertEquals(list2, javaContainer);
    	QMap<String,Object> variantContainer = ContainerTest.toQVariantMap(list2);
    	Map<String,Object> javaStringContainer = new HashMap<>();
    	for(Map.Entry<Short,Double> entry : javaContainer.entrySet()) {
    		javaStringContainer.put(""+entry.getKey(), entry.getValue());
    	}
    	Assert.assertEquals(variantContainer, javaStringContainer);
    	Assert.assertEquals(container.size(), ContainerTest.containerSize(container));
    }
    
    @Test
    public void testQMultiHashShortDouble() {
    	QMultiHash<Short,Double> container = QMultiHash.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4);
    	Map<Short,Double> javaContainer = new TreeMap<>((s1,s2)->s1<=s2?-1:1);
    	ContainerTest.copyQMultiHashShortDoubleToJavaList(container, javaContainer);
    	Assert.assertEquals(container, javaContainer);
    	long hash = ContainerTest.getQMultiHashShortDoubleHash(container);
    	Assert.assertEquals(hash, QtJambiInternal.objectHash(container));
    	QByteArray array = new QByteArray();
    	QDataStream s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	ContainerTest.writeQMultiHashShortDouble(s, container);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	QMultiHash<Short,Double> list2 = new QMultiHash<>(short.class, double.class);
    	list2.readFrom(s);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	
    	array = new QByteArray();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	container.writeTo(s);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	list2 = new QMultiHash<>(short.class, double.class);
    	ContainerTest.readQMultiHashShortDouble(s, list2);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	
    	javaContainer.clear();
    	ContainerTest.copyFromIterable(list2, javaContainer);
    	Assert.assertEquals(list2, javaContainer);
    	QHash<String,Object> variantContainer = ContainerTest.toQVariantHash(list2);
    	if(QtJambiInternal.majorVersion()>5) {
    		Map<String,Object> javaStringContainer = new TreeMap<>((s1, s2)->s1.compareTo(s2)<=0?-1:1);
        	for(Map.Entry<Short,Double> entry : javaContainer.entrySet()) {
        		javaStringContainer.put(""+entry.getKey(), entry.getValue());
        	}
    		Assert.assertEquals(variantContainer, javaStringContainer);
    	}else {
    		List<QPair<String,Object>> javaStringContainer = new ArrayList<>();
	    	for(Map.Entry<Short,Double> entry : javaContainer.entrySet()) {
	    		javaStringContainer.add(new QPair<>(""+entry.getKey(), entry.getValue()));
	    	}
			Assert.assertEquals(variantContainer.size(), javaStringContainer.size());
			for(QPair<String,Object> entry : variantContainer) {
				Assert.assertTrue(javaStringContainer.contains(entry));
			}
    	}
    	Assert.assertEquals(container.size(), ContainerTest.containerSize(container));
    	Assert.assertTrue(ContainerTest.associativeFind(container, (short)1));
    }
    
    @Test
    public void testQMultiMapShortDouble() {
    	QMultiMap<Short,Double> container = QMultiMap.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4);
    	Map<Short,Double> javaContainer = new TreeMap<>((s1,s2)->s1<=s2?-1:1);
    	ContainerTest.copyQMultiMapShortDoubleToJavaList(container, javaContainer);
    	if(QtJambiInternal.majorVersion()>5) {
    		Assert.assertEquals(container, javaContainer);
    	}else {
    		Assert.assertEquals(container.size(), javaContainer.size());
    		for(Map.Entry<Short,Double> entry : javaContainer.entrySet()) {
    			Assert.assertTrue(container.contains(entry.getKey(), entry.getValue()));
    		}
    	}
//    	long hash = ContainerTest.getQMultiMapShortDoubleHash(container);
//    	Assert.assertEquals(hash, QtJambiInternal.objectHash(container));
    	QByteArray array = new QByteArray();
    	QDataStream s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	ContainerTest.writeQMultiMapShortDouble(s, container);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	QMultiMap<Short,Double> list2 = new QMultiMap<>(short.class, double.class);
    	list2.readFrom(s);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	
    	array = new QByteArray();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	container.writeTo(s);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	list2 = new QMultiMap<>(short.class, double.class);
    	ContainerTest.readQMultiMapShortDouble(s, list2);
    	s.dispose();
    	if(QtJambiInternal.majorVersion()>5) {
    		Assert.assertEquals(container, list2);
    	}else{
    		Assert.assertEquals(container.size(), list2.size());
    		for(QPair<Short,Double> entry : list2) {
    			Assert.assertTrue(container.contains(entry.first, entry.second));
    		}
    	}
    	
    	javaContainer.clear();
    	ContainerTest.copyFromIterable(list2, javaContainer);
    	if(QtJambiInternal.majorVersion()>5) {
    		Assert.assertEquals(list2, javaContainer);
    	}else {
    		Assert.assertEquals(list2.size(), javaContainer.size());
    		for(Map.Entry<Short,Double> entry : javaContainer.entrySet()) {
    			Assert.assertTrue(list2.contains(entry.getKey(), entry.getValue()));
    		}
    	}
    	QMap<String,Object> variantContainer = ContainerTest.toQVariantMap(list2);
    	if(QtJambiInternal.majorVersion()>5) {
        	Map<String,Object> javaStringContainer = new TreeMap<>((s1, s2)->s1.compareTo(s2)<=0?-1:1);
        	for(Map.Entry<Short,Double> entry : javaContainer.entrySet()) {
        		javaStringContainer.put(""+entry.getKey(), entry.getValue());
        	}
	    	Assert.assertEquals(variantContainer, javaStringContainer);
		}else {
	    	List<QPair<String,Object>> javaStringContainer = new ArrayList<>();
	    	for(Map.Entry<Short,Double> entry : javaContainer.entrySet()) {
	    		javaStringContainer.add(new QPair<>(""+entry.getKey(), entry.getValue()));
	    	}
			Assert.assertEquals(variantContainer.size(), javaStringContainer.size());
			for(QPair<String,Object> entry : variantContainer) {
				Assert.assertTrue(javaStringContainer.contains(entry));
			}
		}
    	Assert.assertEquals(container.size(), ContainerTest.containerSize(container));
    }
}
