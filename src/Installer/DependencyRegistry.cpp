﻿#include "DependencyRegistry.hpp"

namespace TaleOfTwoWastelands {
    static IContainer m_container {
        get { return _m_container.Value; }
    }

    static Container defaultContainer() {
        return new Container(x => {
                x.ForSingletonOf<ILog>().Use<Log>();
                x.ForSingletonOf<IInstaller>().Use<Installer>();
                x.ForSingletonOf<IPrompts>().Use<Prompts>();
                x.For<IBsaDiff>().Use<BsaDiff>();

                x.For<IPathStore>().Add<SettingsPathStore>();
                x.For<IPathStore>().Add<RegistryPathStore>();
                });
    }
}
