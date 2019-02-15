#include "extensions/filters/http/tap/tap_config_impl.h"

#include "test/extensions/common/tap/common.h"
#include "test/extensions/filters/http/tap/common.h"
#include "test/mocks/common.h"

using testing::_;
using testing::Return;
using testing::ReturnRef;

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace TapFilter {
namespace {

namespace TapCommon = Extensions::Common::Tap;

class HttpPerRequestTapperImplTest : public testing::Test {
public:
  HttpPerRequestTapperImplTest() {
    EXPECT_CALL(*config_, createPerTapSinkHandleManager_(1)).WillOnce(Return(sink_manager_));
    EXPECT_CALL(*config_, numMatchers()).WillOnce(Return(1));
    EXPECT_CALL(*config_, rootMatcher()).WillRepeatedly(ReturnRef(matcher_));
    EXPECT_CALL(matcher_, onNewStream(_)).WillOnce(SaveArgAddress(&statuses_));
    tapper_ = std::make_unique<HttpPerRequestTapperImpl>(config_, 1);
  }

  std::shared_ptr<MockHttpTapConfig> config_{std::make_shared<MockHttpTapConfig>()};
  TapCommon::MockPerTapSinkHandleManager* sink_manager_ =
      new TapCommon::MockPerTapSinkHandleManager;
  std::unique_ptr<HttpPerRequestTapperImpl> tapper_;
  std::vector<TapCommon::MatcherPtr> matchers_{1};
  TapCommon::MockMatcher matcher_{matchers_};
  TapCommon::Matcher::MatchStatusVector* statuses_;
};

TEST_F(HttpPerRequestTapperImplTest, BufferedFlow) {}

} // namespace
} // namespace TapFilter
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
