package io.qt.core;

/**
 * <p>Java wrapper for Qt class QFutureInterface</p>
 */
public final class QFutureInterface<T> extends io.qt.core.QFutureInterfaceBase
    implements java.lang.Cloneable
{
    /**
     * <p>Overloaded factory for {@link #createVoidFutureInterface(io.qt.core.QFutureInterfaceBase.State)}
     *  with <code>initialState = io.qt.core.QFutureInterfaceBase.State.NoState</code>.</p>
     */
    public static QFutureInterface<Void> createVoidFutureInterface() {
    	return createVoidFutureInterface(State.NoState);
    }
    
    public static QFutureInterface<Void> createVoidFutureInterface(State initialState) {
    	return new QFutureInterface<>(true, initialState);
    }
    
    public static QFutureInterface<Void> createVoidFutureInterface(io.qt.core.QFutureInterfaceBase other) {
    	return new QFutureInterface<>(other);
    }
    
    public static <T> QFutureInterface<T> canceledResult()
    { return new QFutureInterface<T>(false, State.resolve(State.Started.value() | State.Finished.value() | State.Canceled.value())); }
	
	
    /**
     * <p>Overloaded constructor for {@link #QFutureInterface(io.qt.core.QFutureInterfaceBase.State)}
     *  with <code>initialState = io.qt.core.QFutureInterfaceBase.State.NoState</code>.</p>
     */
    private QFutureInterface(boolean isVoid) {
        this(isVoid, io.qt.core.QFutureInterfaceBase.State.NoState);
    }
    
    private QFutureInterface(boolean isVoid, io.qt.core.QFutureInterfaceBase.State initialState){
        super((QPrivateConstructor)null);
        initialize_native(this, initialState, isVoid);
    }
    
    /**
     * <p>Overloaded constructor for {@link #QFutureInterface(io.qt.core.QFutureInterfaceBase.State)}
     *  with <code>initialState = io.qt.core.QFutureInterfaceBase.State.NoState</code>.</p>
     */
    public QFutureInterface() {
        this(false, io.qt.core.QFutureInterfaceBase.State.NoState);
    }
    
    public QFutureInterface(io.qt.core.QFutureInterfaceBase.State initialState){
    	this(false, initialState);
    }
    
    private native static <T> void initialize_native(QFutureInterface<T> instance, io.qt.core.QFutureInterfaceBase.State initialState, boolean isVoid);
    
    public QFutureInterface(io.qt.core.QFutureInterface<T> other){
        super((QPrivateConstructor)null);
        initialize_native(this, java.util.Objects.requireNonNull(other));
    }
    
    private QFutureInterface(io.qt.core.QFutureInterfaceBase other){
        super((QPrivateConstructor)null);
        initialize_native(this, java.util.Objects.requireNonNull(other));
    }
    
    private native static <T> void initialize_native(QFutureInterface<T> instance, io.qt.core.QFutureInterfaceBase other);
    
    @io.qt.QtUninvokable
    public final io.qt.core.QFuture<T> future(){
        return new io.qt.core.QFuture<T>(this);
    }
    
    @io.qt.QtUninvokable
    public final boolean reportFinished(T result){
        return reportFinished(QtJambi_LibraryUtilities.internal.nativeId(this), result);
    }
    
    @io.qt.QtUninvokable
    private native boolean reportFinished(long __this__nativeId, T result);
    
    /**
     * <p>Overloaded function for {@link #reportResult(T, int)}
     *  with <code>index = -1</code>.</p>
     */
    @io.qt.QtUninvokable
    public final boolean reportResult(T result) {
        return reportResult(QtJambi_LibraryUtilities.internal.nativeId(this), result, (int)-1);
    }
    
    @io.qt.QtUninvokable
    public final boolean reportResult(T result, int index){
        return reportResult(QtJambi_LibraryUtilities.internal.nativeId(this), result, index);
    }
    
    @io.qt.QtUninvokable
    static native <T> boolean reportResult(long __this__nativeId, T result, int index);
    
    /**
     * <p>Overloaded function for {@link #reportResults(java.util.Collection, int, int)}
     *  with <code>count = -1</code>.</p>
     */
    @io.qt.QtUninvokable
    public final boolean reportResults(java.util.Collection<T> results, int beginIndex) {
        return reportResults(results, beginIndex, (int)-1);
    }
    
    /**
     * <p>Overloaded function for {@link #reportResults(java.util.Collection, int, int)}</p>
     * <p>with: </p><ul>
     * <li><code>beginIndex = -1</code></li>
     * <li><code>count = -1</code></li>
     * </ul>
     */
    @io.qt.QtUninvokable
    public final boolean reportResults(java.util.Collection<T> results) {
        return reportResults(results, (int)-1, (int)-1);
    }
    
    @io.qt.QtUninvokable
    public final boolean reportResults(java.util.Collection<T> results, int beginIndex, int count){
        return reportResults(QtJambi_LibraryUtilities.internal.nativeId(this), results, beginIndex, count);
    }
    
    @io.qt.QtUninvokable
    private native boolean reportResults(long __this__nativeId, java.util.Collection<T> results, int beginIndex, int count);
    
    @io.qt.QtUninvokable
    public final T result(int index){
        return result(QtJambi_LibraryUtilities.internal.nativeId(this), index);
    }
    
    @io.qt.QtUninvokable
    static native <T> T result(long __this__nativeId, int index);
    
    @io.qt.QtUninvokable
    public final io.qt.core.QList<T> results(){
        return results(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    
    @io.qt.QtUninvokable
    static native <T> io.qt.core.QList<T> results(long __this__nativeId);
    
    /**
     * Constructor for internal use only.
     * @param p expected to be <code>null</code>.
     */
    @io.qt.NativeAccess
    private QFutureInterface(QPrivateConstructor p) { super(p); } 
    
    
    @Override
    public QFutureInterface<T> clone() {
        return clone(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    private native QFutureInterface<T> clone(long __this_nativeId);
}
