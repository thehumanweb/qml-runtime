# This file has been generated with tools/build/gen_grpc_cmake.py
cmake_minimum_required(VERSION 2.8.12)
project(grpc)

# Use system SSL
add_definitions(-DTSI_OPENSSL_ALPN_SUPPORT=0)

set(GRPC_DIR ${CMAKE_SOURCE_DIR}/src/3rdparty/grpc)
set(GRPC_INCLUDE_DIR ${GRPC_DIR}/include)

set(PROTOBUF_DIR ${CMAKE_SOURCE_DIR}/src/3rdparty/protobuf)
set(PROTOBUF_INCLUDE_DIR ${PROTOBUF_DIR}/src)

find_path(OPENSSL_INCLUDE_DIR NAMES openssl/ssl.h)
mark_as_advanced(OPENSSL_INCLUDE_DIR)

find_library(OPENSSL_SSL_LIBRARY NAMES ssl)
mark_as_advanced(OPENSSL_SSL_LIBRARY)

find_library(OPENSSL_CRYPTO_LIBRARY NAMES crypto)
mark_as_advanced(OPENSSL_CRYPTO_LIBRARY)

find_library(ZLIB_LIBRARY z)
mark_as_advanced(ZLIB_LIBRARY)

include_directories(${GRPC_DIR} ${OPENSSL_INCLUDE_DIR})





add_library(grpc++ STATIC
    ${GRPC_DIR}/src/cpp/client/secure_channel_arguments.cc
    ${GRPC_DIR}/src/cpp/client/secure_credentials.cc
    ${GRPC_DIR}/src/cpp/common/auth_property_iterator.cc
    ${GRPC_DIR}/src/cpp/common/secure_auth_context.cc
    ${GRPC_DIR}/src/cpp/common/secure_create_auth_context.cc
    ${GRPC_DIR}/src/cpp/server/secure_server_credentials.cc
    ${GRPC_DIR}/src/cpp/client/channel.cc
    ${GRPC_DIR}/src/cpp/client/channel_arguments.cc
    ${GRPC_DIR}/src/cpp/client/client_context.cc
    ${GRPC_DIR}/src/cpp/client/create_channel.cc
    ${GRPC_DIR}/src/cpp/client/create_channel_internal.cc
    ${GRPC_DIR}/src/cpp/client/credentials.cc
    ${GRPC_DIR}/src/cpp/client/generic_stub.cc
    ${GRPC_DIR}/src/cpp/client/insecure_credentials.cc
    ${GRPC_DIR}/src/cpp/common/call.cc
    ${GRPC_DIR}/src/cpp/common/completion_queue.cc
    ${GRPC_DIR}/src/cpp/common/rpc_method.cc
    ${GRPC_DIR}/src/cpp/proto/proto_utils.cc
    ${GRPC_DIR}/src/cpp/server/async_generic_service.cc
    ${GRPC_DIR}/src/cpp/server/create_default_thread_pool.cc
    ${GRPC_DIR}/src/cpp/server/dynamic_thread_pool.cc
    ${GRPC_DIR}/src/cpp/server/fixed_size_thread_pool.cc
    ${GRPC_DIR}/src/cpp/server/insecure_server_credentials.cc
    ${GRPC_DIR}/src/cpp/server/server.cc
    ${GRPC_DIR}/src/cpp/server/server_builder.cc
    ${GRPC_DIR}/src/cpp/server/server_context.cc
    ${GRPC_DIR}/src/cpp/server/server_credentials.cc
    ${GRPC_DIR}/src/cpp/util/byte_buffer.cc
    ${GRPC_DIR}/src/cpp/util/slice.cc
    ${GRPC_DIR}/src/cpp/util/status.cc
    ${GRPC_DIR}/src/cpp/util/string_ref.cc
    ${GRPC_DIR}/src/cpp/util/time.cc
)

target_link_libraries(grpc++
    ${ZLIB_LIBRARY}
    libprotobuf
    gpr
    grpc
)
target_include_directories(grpc++ PUBLIC
    ${GRPC_INCLUDE_DIR}
    ${PROTOBUF_INCLUDE_DIR}
)


add_library(grpc STATIC
    ${GRPC_DIR}/src/core/httpcli/httpcli_security_connector.c
    ${GRPC_DIR}/src/core/security/base64.c
    ${GRPC_DIR}/src/core/security/client_auth_filter.c
    ${GRPC_DIR}/src/core/security/credentials.c
    ${GRPC_DIR}/src/core/security/credentials_metadata.c
    ${GRPC_DIR}/src/core/security/credentials_posix.c
    ${GRPC_DIR}/src/core/security/credentials_win32.c
    ${GRPC_DIR}/src/core/security/google_default_credentials.c
    ${GRPC_DIR}/src/core/security/json_token.c
    ${GRPC_DIR}/src/core/security/jwt_verifier.c
    ${GRPC_DIR}/src/core/security/secure_endpoint.c
    ${GRPC_DIR}/src/core/security/secure_transport_setup.c
    ${GRPC_DIR}/src/core/security/security_connector.c
    ${GRPC_DIR}/src/core/security/security_context.c
    ${GRPC_DIR}/src/core/security/server_auth_filter.c
    ${GRPC_DIR}/src/core/security/server_secure_chttp2.c
    ${GRPC_DIR}/src/core/surface/init_secure.c
    ${GRPC_DIR}/src/core/surface/secure_channel_create.c
    ${GRPC_DIR}/src/core/tsi/fake_transport_security.c
    ${GRPC_DIR}/src/core/tsi/ssl_transport_security.c
    ${GRPC_DIR}/src/core/tsi/transport_security.c
    ${GRPC_DIR}/src/core/census/grpc_context.c
    ${GRPC_DIR}/src/core/census/grpc_filter.c
    ${GRPC_DIR}/src/core/channel/channel_args.c
    ${GRPC_DIR}/src/core/channel/channel_stack.c
    ${GRPC_DIR}/src/core/channel/client_channel.c
    ${GRPC_DIR}/src/core/channel/compress_filter.c
    ${GRPC_DIR}/src/core/channel/connected_channel.c
    ${GRPC_DIR}/src/core/channel/http_client_filter.c
    ${GRPC_DIR}/src/core/channel/http_server_filter.c
    ${GRPC_DIR}/src/core/channel/noop_filter.c
    ${GRPC_DIR}/src/core/client_config/client_config.c
    ${GRPC_DIR}/src/core/client_config/connector.c
    ${GRPC_DIR}/src/core/client_config/lb_policies/pick_first.c
    ${GRPC_DIR}/src/core/client_config/lb_policy.c
    ${GRPC_DIR}/src/core/client_config/resolver.c
    ${GRPC_DIR}/src/core/client_config/resolver_factory.c
    ${GRPC_DIR}/src/core/client_config/resolver_registry.c
    ${GRPC_DIR}/src/core/client_config/resolvers/dns_resolver.c
    ${GRPC_DIR}/src/core/client_config/resolvers/sockaddr_resolver.c
    ${GRPC_DIR}/src/core/client_config/subchannel.c
    ${GRPC_DIR}/src/core/client_config/subchannel_factory.c
    ${GRPC_DIR}/src/core/client_config/subchannel_factory_decorators/add_channel_arg.c
    ${GRPC_DIR}/src/core/client_config/subchannel_factory_decorators/merge_channel_args.c
    ${GRPC_DIR}/src/core/client_config/uri_parser.c
    ${GRPC_DIR}/src/core/compression/algorithm.c
    ${GRPC_DIR}/src/core/compression/message_compress.c
    ${GRPC_DIR}/src/core/debug/trace.c
    ${GRPC_DIR}/src/core/httpcli/format_request.c
    ${GRPC_DIR}/src/core/httpcli/httpcli.c
    ${GRPC_DIR}/src/core/httpcli/parser.c
    ${GRPC_DIR}/src/core/iomgr/alarm.c
    ${GRPC_DIR}/src/core/iomgr/alarm_heap.c
    ${GRPC_DIR}/src/core/iomgr/endpoint.c
    ${GRPC_DIR}/src/core/iomgr/endpoint_pair_posix.c
    ${GRPC_DIR}/src/core/iomgr/endpoint_pair_windows.c
    ${GRPC_DIR}/src/core/iomgr/fd_posix.c
    ${GRPC_DIR}/src/core/iomgr/iocp_windows.c
    ${GRPC_DIR}/src/core/iomgr/iomgr.c
    ${GRPC_DIR}/src/core/iomgr/iomgr_posix.c
    ${GRPC_DIR}/src/core/iomgr/iomgr_windows.c
    ${GRPC_DIR}/src/core/iomgr/pollset_multipoller_with_epoll.c
    ${GRPC_DIR}/src/core/iomgr/pollset_multipoller_with_poll_posix.c
    ${GRPC_DIR}/src/core/iomgr/pollset_posix.c
    ${GRPC_DIR}/src/core/iomgr/pollset_set_posix.c
    ${GRPC_DIR}/src/core/iomgr/pollset_set_windows.c
    ${GRPC_DIR}/src/core/iomgr/pollset_windows.c
    ${GRPC_DIR}/src/core/iomgr/resolve_address_posix.c
    ${GRPC_DIR}/src/core/iomgr/resolve_address_windows.c
    ${GRPC_DIR}/src/core/iomgr/sockaddr_utils.c
    ${GRPC_DIR}/src/core/iomgr/socket_utils_common_posix.c
    ${GRPC_DIR}/src/core/iomgr/socket_utils_linux.c
    ${GRPC_DIR}/src/core/iomgr/socket_utils_posix.c
    ${GRPC_DIR}/src/core/iomgr/socket_windows.c
    ${GRPC_DIR}/src/core/iomgr/tcp_client_posix.c
    ${GRPC_DIR}/src/core/iomgr/tcp_client_windows.c
    ${GRPC_DIR}/src/core/iomgr/tcp_posix.c
    ${GRPC_DIR}/src/core/iomgr/tcp_server_posix.c
    ${GRPC_DIR}/src/core/iomgr/tcp_server_windows.c
    ${GRPC_DIR}/src/core/iomgr/tcp_windows.c
    ${GRPC_DIR}/src/core/iomgr/time_averaged_stats.c
    ${GRPC_DIR}/src/core/iomgr/udp_server.c
    ${GRPC_DIR}/src/core/iomgr/wakeup_fd_eventfd.c
    ${GRPC_DIR}/src/core/iomgr/wakeup_fd_nospecial.c
    ${GRPC_DIR}/src/core/iomgr/wakeup_fd_pipe.c
    ${GRPC_DIR}/src/core/iomgr/wakeup_fd_posix.c
    ${GRPC_DIR}/src/core/json/json.c
    ${GRPC_DIR}/src/core/json/json_reader.c
    ${GRPC_DIR}/src/core/json/json_string.c
    ${GRPC_DIR}/src/core/json/json_writer.c
    ${GRPC_DIR}/src/core/profiling/basic_timers.c
    ${GRPC_DIR}/src/core/profiling/stap_timers.c
    ${GRPC_DIR}/src/core/surface/byte_buffer.c
    ${GRPC_DIR}/src/core/surface/byte_buffer_queue.c
    ${GRPC_DIR}/src/core/surface/byte_buffer_reader.c
    ${GRPC_DIR}/src/core/surface/call.c
    ${GRPC_DIR}/src/core/surface/call_details.c
    ${GRPC_DIR}/src/core/surface/call_log_batch.c
    ${GRPC_DIR}/src/core/surface/channel.c
    ${GRPC_DIR}/src/core/surface/channel_connectivity.c
    ${GRPC_DIR}/src/core/surface/channel_create.c
    ${GRPC_DIR}/src/core/surface/completion_queue.c
    ${GRPC_DIR}/src/core/surface/event_string.c
    ${GRPC_DIR}/src/core/surface/init.c
    ${GRPC_DIR}/src/core/surface/lame_client.c
    ${GRPC_DIR}/src/core/surface/metadata_array.c
    ${GRPC_DIR}/src/core/surface/server.c
    ${GRPC_DIR}/src/core/surface/server_chttp2.c
    ${GRPC_DIR}/src/core/surface/server_create.c
    ${GRPC_DIR}/src/core/surface/surface_trace.c
    ${GRPC_DIR}/src/core/surface/version.c
    ${GRPC_DIR}/src/core/transport/chttp2/alpn.c
    ${GRPC_DIR}/src/core/transport/chttp2/bin_encoder.c
    ${GRPC_DIR}/src/core/transport/chttp2/frame_data.c
    ${GRPC_DIR}/src/core/transport/chttp2/frame_goaway.c
    ${GRPC_DIR}/src/core/transport/chttp2/frame_ping.c
    ${GRPC_DIR}/src/core/transport/chttp2/frame_rst_stream.c
    ${GRPC_DIR}/src/core/transport/chttp2/frame_settings.c
    ${GRPC_DIR}/src/core/transport/chttp2/frame_window_update.c
    ${GRPC_DIR}/src/core/transport/chttp2/hpack_parser.c
    ${GRPC_DIR}/src/core/transport/chttp2/hpack_table.c
    ${GRPC_DIR}/src/core/transport/chttp2/huffsyms.c
    ${GRPC_DIR}/src/core/transport/chttp2/incoming_metadata.c
    ${GRPC_DIR}/src/core/transport/chttp2/parsing.c
    ${GRPC_DIR}/src/core/transport/chttp2/status_conversion.c
    ${GRPC_DIR}/src/core/transport/chttp2/stream_encoder.c
    ${GRPC_DIR}/src/core/transport/chttp2/stream_lists.c
    ${GRPC_DIR}/src/core/transport/chttp2/stream_map.c
    ${GRPC_DIR}/src/core/transport/chttp2/timeout_encoding.c
    ${GRPC_DIR}/src/core/transport/chttp2/varint.c
    ${GRPC_DIR}/src/core/transport/chttp2/writing.c
    ${GRPC_DIR}/src/core/transport/chttp2_transport.c
    ${GRPC_DIR}/src/core/transport/connectivity_state.c
    ${GRPC_DIR}/src/core/transport/metadata.c
    ${GRPC_DIR}/src/core/transport/stream_op.c
    ${GRPC_DIR}/src/core/transport/transport.c
    ${GRPC_DIR}/src/core/transport/transport_op_string.c
    ${GRPC_DIR}/src/core/census/context.c
    ${GRPC_DIR}/src/core/census/initialize.c
    ${GRPC_DIR}/src/core/census/operation.c
    ${GRPC_DIR}/src/core/census/tracing.c
)

target_link_libraries(grpc
    ${ZLIB_LIBRARY}
    ${OPENSSL_SSL_LIBRARY}
    ${OPENSSL_CRYPTO_LIBRARY}
    gpr
)
target_include_directories(grpc PUBLIC
    ${GRPC_INCLUDE_DIR}
    ${PROTOBUF_INCLUDE_DIR}
)


add_library(gpr STATIC
    ${GRPC_DIR}/src/core/support/alloc.c
    ${GRPC_DIR}/src/core/support/cmdline.c
    ${GRPC_DIR}/src/core/support/cpu_iphone.c
    ${GRPC_DIR}/src/core/support/cpu_linux.c
    ${GRPC_DIR}/src/core/support/cpu_posix.c
    ${GRPC_DIR}/src/core/support/cpu_windows.c
    ${GRPC_DIR}/src/core/support/env_linux.c
    ${GRPC_DIR}/src/core/support/env_posix.c
    ${GRPC_DIR}/src/core/support/env_win32.c
    ${GRPC_DIR}/src/core/support/file.c
    ${GRPC_DIR}/src/core/support/file_posix.c
    ${GRPC_DIR}/src/core/support/file_win32.c
    ${GRPC_DIR}/src/core/support/histogram.c
    ${GRPC_DIR}/src/core/support/host_port.c
    ${GRPC_DIR}/src/core/support/log.c
    ${GRPC_DIR}/src/core/support/log_android.c
    ${GRPC_DIR}/src/core/support/log_linux.c
    ${GRPC_DIR}/src/core/support/log_posix.c
    ${GRPC_DIR}/src/core/support/log_win32.c
    ${GRPC_DIR}/src/core/support/murmur_hash.c
    ${GRPC_DIR}/src/core/support/slice.c
    ${GRPC_DIR}/src/core/support/slice_buffer.c
    ${GRPC_DIR}/src/core/support/stack_lockfree.c
    ${GRPC_DIR}/src/core/support/string.c
    ${GRPC_DIR}/src/core/support/string_posix.c
    ${GRPC_DIR}/src/core/support/string_win32.c
    ${GRPC_DIR}/src/core/support/subprocess_posix.c
    ${GRPC_DIR}/src/core/support/sync.c
    ${GRPC_DIR}/src/core/support/sync_posix.c
    ${GRPC_DIR}/src/core/support/sync_win32.c
    ${GRPC_DIR}/src/core/support/thd.c
    ${GRPC_DIR}/src/core/support/thd_posix.c
    ${GRPC_DIR}/src/core/support/thd_win32.c
    ${GRPC_DIR}/src/core/support/time.c
    ${GRPC_DIR}/src/core/support/time_posix.c
    ${GRPC_DIR}/src/core/support/time_win32.c
    ${GRPC_DIR}/src/core/support/tls_pthread.c
)

target_link_libraries(gpr
    ${ZLIB_LIBRARY}
)
target_include_directories(gpr PUBLIC
    ${GRPC_INCLUDE_DIR}
    ${PROTOBUF_INCLUDE_DIR}
)


add_library(grpc_plugin_support STATIC
    ${GRPC_DIR}/src/compiler/cpp_generator.cc
    ${GRPC_DIR}/src/compiler/csharp_generator.cc
    ${GRPC_DIR}/src/compiler/objective_c_generator.cc
    ${GRPC_DIR}/src/compiler/python_generator.cc
    ${GRPC_DIR}/src/compiler/ruby_generator.cc
)

target_link_libraries(grpc_plugin_support
    ${ZLIB_LIBRARY}
    libprotoc
    libprotobuf
)
target_include_directories(grpc_plugin_support PUBLIC
    ${GRPC_INCLUDE_DIR}
    ${PROTOBUF_INCLUDE_DIR}
)



add_executable(grpc_cpp_plugin
    ${GRPC_DIR}/src/compiler/cpp_plugin.cc
)
target_link_libraries(grpc_cpp_plugin
    ${ZLIB_LIBRARY}
    libprotoc
    libprotobuf
    grpc_plugin_support
)





