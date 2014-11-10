/* analytics_test.cc

    Analytics plugin test   
*/


#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include "jml/arch/timers.h"

#include "rtbkit/plugins/analytics/analytics.h"

using namespace std;
using namespace ML;
using namespace Datacratic;

void setUpEndpoint(shared_ptr<AnalyticsRestEndpoint> & analyticsEndpoint)
{
    auto proxies = std::make_shared<ServiceProxies> ();
    analyticsEndpoint = make_shared<AnalyticsRestEndpoint> (proxies, "analytics");
    analyticsEndpoint->init();
    analyticsEndpoint->bindTcp(40000);
    analyticsEndpoint->start();
}

void setUpClient(shared_ptr<AnalyticsClient> & analyticsClient)
{
    analyticsClient = make_shared<AnalyticsClient> ();
    analyticsClient->init("http://127.0.0.1:40000");
    analyticsClient->start();
}

BOOST_AUTO_TEST_CASE( analytics_simple_message_test )
{
    shared_ptr<AnalyticsRestEndpoint> analyticsEndpoint;
    setUpEndpoint(analyticsEndpoint);

    shared_ptr<AnalyticsClient> analyticsClient;
    setUpClient(analyticsClient);

    // must sleep to let the heartbeat make a connection,
    // else it will not send message because the connection
    // is not established.
    ML::sleep(2.0);

    analyticsClient->publish("Test", "message", "channel is ignored");

    ML::sleep(0.5);

    analyticsEndpoint->enableChannel("Test");

    ML::sleep(0.5);

    analyticsClient->publish("Test", "message", "channel is enabled");

    ML::sleep(0.5);

    analyticsEndpoint->enableAllChannels();

    ML::sleep(0.5);

    analyticsClient->publish("Test2", "all", "channels enabled");

    ML::sleep(0.5);

    analyticsEndpoint->disableAllChannels();

    ML::sleep(0.5);

    analyticsClient->publish("Test2", "all", "channels now disabled");

    ML::sleep(0.5);

    analyticsClient->shutdown();
    analyticsEndpoint->shutdown();
}
