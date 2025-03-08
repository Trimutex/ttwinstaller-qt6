
namespace TaleOfTwoWastelandsProgress {
        /// <summary>Initializes the <see cref="Progress{T}"/>.</summary>
        Progress() {
            //OFFICIAL documentation:
            // // Capture the current synchronization context.  "current" is determined by CurrentNoFlow,
            // // which doesn't consider the [....] ctx flown with an ExecutionContext, avoiding
            // // [....] ctx reference identity issues where the [....] ctx for one thread could be Current on another.
            // // If there is no current context, we use a default instance targeting the ThreadPool.

            //CurrentNoFlow is NOT implemented prior to .NET 4.5, so Current is used instead
            //THIS WILL CHANGE SYNCHRONIZATION BEHAVIOR!!
            m_synchronizationContext = SynchronizationContext.Current/*NoFlow*/ ?? ProgressStatics.DefaultContext;
            Contract.Assert(m_synchronizationContext != null);
            m_invokeHandlers = new SendOrPostCallback(InvokeHandlers);
        }

        /// <summary>Initializes the <see cref="Progress{T}"/> with the specified callback.</summary>
        /// <param name="handler">
        /// A handler to invoke for each reported progress value.  This handler will be invoked
        /// in addition to any delegates registered with the <see cref="ProgressChanged"/> event.
        /// Depending on the <see cref="System.Threading.SynchronizationContext"/> instance captured by 
        /// the <see cref="Progress"/> at construction, it's possible that this handler instance
        /// could be invoked concurrently with itself.
        /// </param>
        /// <exception cref="System.ArgumentNullException">The <paramref name="handler"/> is null (Nothing in Visual Basic).</exception>
        Progress(Action<T> handler) : this() {
            if (handler == null) throw new ArgumentNullException("handler");
            m_handler = handler;
        }

        /// <summary>Reports a progress change.</summary>
        /// <param name="value">The value of the updated progress.</param>
        virtual void onReport(T value) {
            // If there's no handler, don't bother going through the [....] context.
            // Inside the callback, we'll need to check again, in case 
            // an event handler is removed between now and then.
            Action<T> handler = m_handler;
            GenericEventHandler<T> changedEvent = ProgressChanged;
            if (handler != null || changedEvent != null) {
                // Post the processing to the [....] context.
                // (If T is a value type, it will get boxed here.)
                m_synchronizationContext.Post(m_invokeHandlers, value);
            }
        }

        /// <summary>Reports a progress change.</summary>
        /// <param name="value">The value of the updated progress.</param>
        void IProgress<T>.report(T value) {
            OnReport(value);
        }

        /// <summary>Invokes the action and event callbacks.</summary>
        /// <param name="state">The progress value.</param>
        void invokeHandlers(object state) {
            T value = (T)state;

            Action<T> handler = m_handler;
            GenericEventHandler<T> changedEvent = ProgressChanged;

            if (handler != null) handler(value);
            if (changedEvent != null) changedEvent(this, value);
        }
}
