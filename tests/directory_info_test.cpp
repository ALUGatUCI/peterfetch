#include "directory_info.hpp"

#include <string>
#include <sstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "http_client.hpp"

using ::testing::Return;
using ::testing::Exactly;
using namespace std::string_literals;

class MockHttpClient : public HttpClient {
public:
    MOCK_METHOD(cpr::Response, get, (const cpr::Url& url, bool redirect),
                (override));
};

constexpr const char *PROPER_RESPONSE = R"(
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
        "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html lang="en" xmlns="http://www.w3.org/1999/xhtml" class="gr__directory_uci_edu">
    <head>
        <title>UCI Directory</title>
        <meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
        <meta content="width=device-width, initial-scale=1" name="viewport" />
    </head>
    <body>
UCInetID: peter<br/>
Name: Peter Anteater<br/>
E-mail: <script type="text/javascript">document.write(atob('cGV0ZXJAdWNpLmVkdQ=='));</script><br/>
Delivery Point: <script type="text/javascript">document.write(atob('cGV0ZXJAZ21haWxib3guZXMudWNpLmVkdQ=='));</script><br/>
Major: CmptSci<br/>
Student's Level: SO<br/>
    </body>
</html>
)";

TEST(DirectoryInfo, BeginsUnpopulated) {
    DirectoryInfo info("peter");
    EXPECT_FALSE(info.populated());
}

TEST(DirectoryInfo, UnpopulatedAccessThrows) {
    DirectoryInfo info("peter");
    std::string tmp;
    StudentLevel level_tmp;

    EXPECT_THROW(tmp = info.netid(), std::runtime_error);
    EXPECT_THROW(tmp = info.name(), std::runtime_error);
    EXPECT_THROW(tmp = info.major(), std::runtime_error);
    EXPECT_THROW(level_tmp = info.level(), std::runtime_error);
}

TEST(DirectoryInfo, RedirectsBecomeMissingPeople) {
    MockHttpClient client;
    cpr::Response missing_response;
    missing_response.status_code = 302;
    cpr::Url expected_url { DIRECTORY_BASE_URL + "peter.txt"s };

    EXPECT_CALL(client, get(expected_url, false))
        .Times(Exactly(1))
        .WillRepeatedly(Return(missing_response));

    DirectoryInfo info("peter", DIRECTORY_BASE_URL, &client);
    DirectoryFetchResult result = info.fetch();
    EXPECT_EQ(result, DirectoryFetchResult::DOESNT_EXIST);
}

TEST(DirectoryInfo, FetchingPopulates) {
    MockHttpClient client;
    cpr::Response filled_response;
    filled_response.status_code = 200;
    filled_response.text = PROPER_RESPONSE;
    cpr::Url expected_url { DIRECTORY_BASE_URL + "peter.txt"s };

    EXPECT_CALL(client, get(expected_url, false))
        .Times(Exactly(1))
        .WillRepeatedly(Return(filled_response));

    DirectoryInfo info("peter", DIRECTORY_BASE_URL, &client);
    DirectoryFetchResult result = info.fetch();
    EXPECT_EQ(result, DirectoryFetchResult::OK);
    EXPECT_TRUE(info.populated());
}

TEST(DirectoryInfo, PrintingUnpopulated) {
    DirectoryInfo info("peter");
    std::stringstream s;

    info.print(s);
    EXPECT_FALSE(info.populated());
    EXPECT_TRUE(s.str().find("unpopulated"));
}

TEST(DirectoryInfo, PrintingPopulated) {
    MockHttpClient client;
    cpr::Response filled_response;
    filled_response.status_code = 200;
    filled_response.text = PROPER_RESPONSE;
    cpr::Url expected_url { DIRECTORY_BASE_URL + "peter.txt"s };

    EXPECT_CALL(client, get(expected_url, false))
        .Times(Exactly(1))
        .WillRepeatedly(Return(filled_response));

    DirectoryInfo info("peter", DIRECTORY_BASE_URL, &client);
    DirectoryFetchResult result = info.fetch();
    EXPECT_EQ(result, DirectoryFetchResult::OK);

    std::stringstream s;
    info.print(s);

    // The exact formatting doesn't matter, just ensure that all the right
    // information appears
    EXPECT_TRUE(s.str().find("peter"));
    EXPECT_TRUE(s.str().find("Peter Anteater"));
    EXPECT_TRUE(s.str().find("CompSci"));
    EXPECT_TRUE(s.str().find("SOPHOMORE"));
}
