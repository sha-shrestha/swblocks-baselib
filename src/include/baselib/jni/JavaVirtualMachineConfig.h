/*
 * This file is part of the swblocks-baselib library.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __BL_JNI_JAVAVIRTUALMACHINECONFIG_H_
#define __BL_JNI_JAVAVIRTUALMACHINECONFIG_H_

#include <baselib/core/ObjModel.h>
#include <baselib/core/BaseIncludes.h>

#define JVM_CONFIG_STRING_PROPERTY( property, Property, option, defaultValue )          \
    private:                                                                            \
                                                                                        \
        std::string                                 m_##property = defaultValue;        \
                                                                                        \
        void addOption##Property( SAA_inout std::vector< std::string >& options ) const \
        {                                                                               \
            if( ! m_##property.empty() )                                                \
            {                                                                           \
                options.push_back( option + m_##property );                             \
            }                                                                           \
        }                                                                               \
                                                                                        \
    public:                                                                             \
                                                                                        \
        const std::string& get##Property() const NOEXCEPT                               \
        {                                                                               \
            return m_##property;                                                        \
        }                                                                               \
                                                                                        \
        void set##Property( SAA_in std::string&& value ) NOEXCEPT                       \
        {                                                                               \
            m_##property = BL_PARAM_FWD( value );                                       \
        }                                                                               \
                                                                                        \
    private:                                                                            \

#define JVM_CONFIG_BOOL_PROPERTY( property, Property, option, defaultValue )            \
    private:                                                                            \
                                                                                        \
        bool                                        m_##property = defaultValue;        \
                                                                                        \
        void addOption##Property( SAA_inout std::vector< std::string >& options ) const \
        {                                                                               \
            if( m_##property )                                                          \
            {                                                                           \
                options.push_back( option );                                            \
            }                                                                           \
        }                                                                               \
                                                                                        \
    public:                                                                             \
                                                                                        \
        bool get##Property() const NOEXCEPT                                             \
        {                                                                               \
            return m_##property;                                                        \
        }                                                                               \
                                                                                        \
        void set##Property( SAA_in const bool value ) NOEXCEPT                          \
        {                                                                               \
            m_##property = value;                                                       \
        }                                                                               \
                                                                                        \
    private:                                                                            \

namespace bl
{
    namespace jni
    {
        /**
         * @brief class JavaVirtualMachineConfig
         */

        template
        <
            typename E = void
        >
        class JavaVirtualMachineConfigT
        {
        private:

            std::string                                             m_libraryPath;
            std::vector< std::string >                              m_options;

            JVM_CONFIG_STRING_PROPERTY  ( classPath,                ClassPath,              "-Djava.class.path=",       ""      )
            JVM_CONFIG_STRING_PROPERTY  ( threadStackSize,          ThreadStackSize,        "-Xss",                     ""      )
            JVM_CONFIG_STRING_PROPERTY  ( initialHeapSize,          InitialHeapSize,        "-Xms",                     "512M"  )
            JVM_CONFIG_STRING_PROPERTY  ( maximumHeapSize,          MaximumHeapSize,        "-Xmx",                     "4G"    )

            JVM_CONFIG_STRING_PROPERTY  ( debugPort,                DebugPort,
                                                    "-agentlib:jdwp=transport=dt_socket,server=y,suspend=y,address=",   ""      )

            JVM_CONFIG_BOOL_PROPERTY    ( checkJni,                 CheckJni,               "-Xcheck:jni",              false   )
            JVM_CONFIG_BOOL_PROPERTY    ( verboseJni,               VerboseJni,             "-verbose:jni",             false   )
            JVM_CONFIG_BOOL_PROPERTY    ( printGCDetails,           PrintGCDetails,         "-XX:+PrintGCDetails",      false   )
            JVM_CONFIG_BOOL_PROPERTY    ( traceClassLoading,        TraceClassLoading,      "-XX:+TraceClassLoading",   false   )
            JVM_CONFIG_BOOL_PROPERTY    ( traceClassUnloading,      TraceClassUnloading,    "-XX:+TraceClassUnloading", false   )

            /*
             * To avoid well-known problems with /dev/random used as default random generator, which causes JDBC
             * connection errors among other issues, below option will be set to 'true' by default on Linux only
             */
            JVM_CONFIG_BOOL_PROPERTY    ( useDevUrandom,            UseDevUrandom,
                                                                            "-Djava.security.egd=file:/dev/urandom",    os::onLinux() )

        public:

            const std::string& getLibraryPath() const NOEXCEPT
            {
                return m_libraryPath;
            }

            void setLibraryPath( SAA_in std::string&& libraryPath ) NOEXCEPT
            {
                m_libraryPath = BL_PARAM_FWD( libraryPath );
            }

            void addOption( SAA_in std::string&& option )
            {
                m_options.emplace_back( BL_PARAM_FWD( option ) );
            }

            std::vector< std::string > getJavaVMOptions()
            {
                std::vector< std::string > options;

                addOptionClassPath( options );
                addOptionThreadStackSize( options );
                addOptionInitialHeapSize( options );
                addOptionMaximumHeapSize( options );
                addOptionDebugPort( options );

                addOptionUseDevUrandom( options );
                addOptionCheckJni( options );
                addOptionVerboseJni( options );
                addOptionPrintGCDetails( options );
                addOptionTraceClassLoading( options );
                addOptionTraceClassUnloading( options );

                for( const auto& option : m_options )
                {
                    options.push_back( option );
                }

                return options;
            }
        };

        typedef JavaVirtualMachineConfigT<> JavaVirtualMachineConfig;

    } // jni

} // bl

#undef JVM_CONFIG_BOOL_PROPERTY
#undef JVM_CONFIG_STRING_PROPERTY

#endif /* __BL_JNI_JAVAVIRTUALMACHINECONFIG_H_ */
