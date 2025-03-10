#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: internal class
	static class DependencyRegistry {
    public:
        static IContainer m_container;

    private:
        // NOTE: read-only block
        static Lazy<Container> _m_container =
            new Lazy<Container>(defaultContainer, LazyThreadSafetyMode.ExecutionAndPublication);

        static Container defaultContainer();
    }
}
