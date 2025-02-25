#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: internal class
	static class DependencyRegistry {
    public:
        static IContainer Container;

    private:
        // NOTE: read-only block
        static Lazy<Container> _container =
            new Lazy<Container>(defaultContainer, LazyThreadSafetyMode.ExecutionAndPublication);

        static Container defaultContainer();
    }
}
