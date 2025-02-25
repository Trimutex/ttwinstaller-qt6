#pragma once

namespace TaleOfTwoWastelandsProgress {
    // NOTE: public class
    // NOTE: interface class
    IProgress<in T> {
        void Report(T value);
    }

    /// <summary>
    /// Provides an IProgress{T} that invokes callbacks for each reported progress value.
    /// </summary>
    /// <typeparam name="T">Specifies the type of the progress report value.</typeparam>
    /// <remarks>
    /// Any handler provided to the constructor or event handlers registered with
    /// the <see cref="ProgressChanged"/> event are invoked through a 
    /// <see cref="System.Threading.SynchronizationContext"/> instance captured
    /// when the instance is constructed.  If there is no current SynchronizationContext
    /// at the time of construction, the callbacks will be invoked on the ThreadPool.
    /// </remarks>
    // NOTE: public class
    class Progress<T> : IProgress<T> {
    public:
        Progress();
        Progress(Action<T> handler);

/* VESTIGIAL MACRO
 *  [Serializable]
 *  [ComVisible(true)]
 */
        delegate void GenericEventHandler<P>(Object sender, P e);

        /// <summary>Raised for each reported progress value.</summary>
        /// <remarks>
        /// Handlers registered with this event will be invoked on the 
        /// <see cref="System.Threading.SynchronizationContext"/> captured when the instance was constructed.
        /// </remarks>
        event GenericEventHandler<T> ProgressChanged;

    protected:
        virtual void OnReport(T value);

    private:
        // NOTE: read-only block
        /// <summary>The synchronization context captured upon construction.  This will never be null.</summary>
        SynchronizationContext m_synchronizationContext;
        /// <summary>The handler specified to the constructor.  This may be null.</summary>
        Action<T> m_handler;
        /// <summary>A cached delegate used to post invocation to the synchronization context.</summary>
        SendOrPostCallback m_invokeHandlers;

        void IProgress<T>.Report(T value);

        void InvokeHandlers(object state);
    }

    /// <summary>Holds static values for <see cref="Progress{T}"/>.</summary>
    /// <remarks>This avoids one static instance per type T.</remarks>
    // NOTE: originally internal
    static class ProgressStatics {
        /// <summary>A default synchronization context that targets the ThreadPool.</summary>
        // NOTE: originally internal
        // NOTE: read-only block
        static SynchronizationContext DefaultContext = new SynchronizationContext();
    }
}
