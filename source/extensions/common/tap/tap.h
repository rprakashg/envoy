#pragma once

#include "envoy/common/pure.h"
#include "envoy/data/tap/v2alpha/wrapper.pb.h"
#include "envoy/http/header_map.h"
#include "envoy/service/tap/v2alpha/common.pb.h"

#include "extensions/common/tap/tap_matcher.h"

#include "absl/strings/string_view.h"

namespace Envoy {
namespace Extensions {
namespace Common {
namespace Tap {

using TraceWrapperSharedPtr = std::shared_ptr<envoy::data::tap::v2alpha::TraceWrapper>;
inline TraceWrapperSharedPtr makeTraceWrapper() {
  return std::make_shared<envoy::data::tap::v2alpha::TraceWrapper>();
}

/**
 * A handle for a per-tap sink. This allows submitting either a single buffered trace, or a series
 * of trace segments that the sink can aggregate in whatever way it chooses.
 */
class PerTapSinkHandle {
public:
  virtual ~PerTapSinkHandle() = default;

  /**
   * Send a trace wrapper to the sink. This may be a fully buffered trace or a segment of a larger
   * trace depending on the contents of the wrapper.
   * @param trace supplies the trace to send.
   * @param format supplies the output format to use.
   */
  virtual void submitTrace(const TraceWrapperSharedPtr& trace,
                           envoy::service::tap::v2alpha::OutputSink::Format format) PURE;
};

using PerTapSinkHandlePtr = std::unique_ptr<PerTapSinkHandle>;

/**
 * Sink for sending tap messages.
 */
class Sink {
public:
  virtual ~Sink() = default;

  /**
   * Create a per tap sink handle for use in submitting either buffered traces or trace segments.
   * @param trace_id supplies a locally unique trace ID. Some sinks use this for output generation.
   */
  virtual PerTapSinkHandlePtr createPerTapSinkHandle(uint64_t trace_id) PURE;
};

using SinkPtr = std::unique_ptr<Sink>;

/**
 * Generic configuration for a tap extension (filter, transport socket, etc.).
 */
class ExtensionConfig {
public:
  virtual ~ExtensionConfig() = default;

  /**
   * @return the ID to use for admin extension configuration tracking (if applicable).
   */
  virtual const absl::string_view adminId() PURE;

  /**
   * Clear any active tap configuration.
   */
  virtual void clearTapConfig() PURE;

  /**
   * Install a new tap configuration.
   * @param proto_config supplies the generic tap config to install. Not all configuration fields
   *        may be applicable to an extension (e.g. HTTP fields). The extension is free to fail
   *        the configuration load via exception if it wishes.
   * @param admin_streamer supplies the singleton admin sink to use for output if the configuration
   *        specifies that output type. May not be used if the configuration does not specify
   *        admin output. May be nullptr if admin is not used to supply the config.
   */
  virtual void newTapConfig(envoy::service::tap::v2alpha::TapConfig&& proto_config,
                            Sink* admin_streamer) PURE;
};

// fixfix
class PerTapSinkHandleManager {
public:
  virtual ~PerTapSinkHandleManager() = default;

  // fixfix
  virtual void submitTrace(const TraceWrapperSharedPtr& trace) PURE;
};

using PerTapSinkHandleManagerPtr = std::unique_ptr<PerTapSinkHandleManager>;

/**
 * Abstract tap configuration base class. Used for type safety.
 */
class TapConfig {
public:
  virtual ~TapConfig() = default;

  // fixfix
  virtual PerTapSinkHandleManagerPtr createPerTapSinkHandleManager(uint64_t trace_id) PURE;

  // fixfix
  virtual uint32_t maxBufferedRxBytes() const PURE;
  virtual uint32_t maxBufferedTxBytes() const PURE;
  virtual size_t numMatchers() const PURE;
  virtual const Matcher& rootMatcher() const PURE;
  virtual bool streaming() const PURE;
};

using TapConfigSharedPtr = std::shared_ptr<TapConfig>;

/**
 * Abstract tap configuration factory. Given a new generic tap configuration, produces an
 * extension specific tap configuration.
 */
class TapConfigFactory {
public:
  virtual ~TapConfigFactory() = default;

  /**
   * @return a new configuration given a raw tap service config proto. See
   * ExtensionConfig::newTapConfig() for param info.
   */
  virtual TapConfigSharedPtr
  createConfigFromProto(envoy::service::tap::v2alpha::TapConfig&& proto_config,
                        Sink* admin_streamer) PURE;
};

using TapConfigFactoryPtr = std::unique_ptr<TapConfigFactory>;

} // namespace Tap
} // namespace Common
} // namespace Extensions
} // namespace Envoy
