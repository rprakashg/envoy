#include "extensions/common/tap/tap.h"

#include "gmock/gmock.h"

namespace Envoy {
namespace Extensions {
namespace Common {
namespace Tap {

class MockPerTapSinkHandleManager : public PerTapSinkHandleManager {
public:
  MOCK_METHOD1(submitTrace, void(const TraceWrapperSharedPtr& trace));
};

class MockMatcher : public Matcher {
public:
  using Matcher::Matcher;

  MOCK_CONST_METHOD1(onNewStream, void(MatchStatusVector& statuses));
  MOCK_CONST_METHOD2(onHttpRequestHeaders,
                     void(const Http::HeaderMap& request_headers, MatchStatusVector& statuses));
  MOCK_CONST_METHOD2(onHttpRequestTrailers,
                     void(const Http::HeaderMap& request_trailers, MatchStatusVector& statuses));
  MOCK_CONST_METHOD2(onHttpResponseHeaders,
                     void(const Http::HeaderMap& response_headers, MatchStatusVector& statuses));
  MOCK_CONST_METHOD2(onHttpResponseTrailers,
                     void(const Http::HeaderMap& response_trailers, MatchStatusVector& statuses));
};

} // namespace Tap
} // namespace Common
} // namespace Extensions
} // namespace Envoy
