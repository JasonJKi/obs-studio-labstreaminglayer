set (liblsl_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
set (liblsl_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set (liblsl_LIBRARIES liblsl PARENT_SCOPE)
include_directories (${Boost_INCLUDE_DIRS})

set (liblsl_SOURCES
  api_config.cpp
  common.cpp
  consumer_queue.cpp
  data_receiver.cpp
  dllmain.cpp
  info_receiver.cpp
  inlet_connection.cpp
  lsl_continuous_resolver_c.cpp
  lsl_freefuncs_c.cpp
  lsl_inlet_c.cpp
  lsl_outlet_c.cpp
  lsl_streaminfo_c.cpp
  lsl_xml_element_c.cpp
  resolve_attempt_udp.cpp
  resolver_impl.cpp
  sample.cpp
  send_buffer.cpp
  socket_utils.cpp
  stream_info_impl.cpp
  stream_outlet_impl.cpp
  tcp_server.cpp
  time_postprocessor.cpp
  time_receiver.cpp
  udp_server.cpp
  pugixml/pugixml.cpp
  legacy/legacy_abi.cpp
)

set (liblsl_HEADERS
  lsl_c.h
  lsl_cpp.h
  api_config.h
  cancellable_streambuf.h
  cancellation.h
  common.h
  consumer_queue.h
  data_receiver.h
  info_receiver.h
  inlet_connection.h
  resolve_attempt_udp.h
  resolver_impl.h
  sample.h
  send_buffer.h
  socket_utils.h
  stream_info_impl.h
  stream_inlet_impl.h
  stream_outlet_impl.h
  tcp_server.h
  time_postprocessor.h
  time_receiver.h
  udp_server.h
)

message (${Boost_LIBRARIES})
set(target lsl64)
if (NOT (CMAKE_SIZEOF_VOID_P EQUAL 8))
  set(target lsl32)
endif()

if (BUILD_SHARED)
  add_library (${target} SHARED ${liblsl_SOURCES} ${liblsl_HEADERS})
  target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../external)
  target_link_libraries (${target} ${Boost_LIBRARIES})
  set_target_properties(${target}
    PROPERTIES
        VERSION ${liblsl_VERSION_MAJOR}.${liblsl_VERSION_MINOR}.${liblsl_VERSION_PATCH}
        SOVERSION ${liblsl_VERSION_MAJOR}.0.0
  )

  # TODO: Need to check if all flags are necessary or if some flags are missing
  if (WIN32)
    if (MINGW)
      target_link_libraries (${target} winmm ws2_32 wsock32)
    endif ()
    set_property(
        TARGET ${target}
        PROPERTY
            COMPILE_DEFINITIONS
                BOOST_ALL_NO_LIB
                BOOST_THREAD_BUILD_LIB
                LIBLSL_EXPORTS
                _SCL_SECURE_NO_WARNINGS
                _CRT_SECURE_NO_WARNINGS
    )
    set_target_properties(${target}
        PROPERTIES
            PREFIX lib
            IMPORT_PREFIX lib
    )
  elseif (APPLE)
    target_link_libraries (${target} pthread)
  elseif (UNIX)
    set_property (TARGET ${target} PROPERTY COMPILE_FLAGS "-fPIC -Wl,--exclude-libs,ALL")
    target_link_libraries (${target} rt pthread)
  endif ()

  if (CMAKE_VERSION VERSION_GREATER 2.8.8)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5)
        set_property (TARGET ${target} PROPERTY COMPILE_FLAGS " -std=c++0x" APPEND_STRING)
      endif ()
    endif ()
  endif()

endif (BUILD_SHARED)
