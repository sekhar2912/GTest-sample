#include "json.hpp"
#include "gtest/gtest.h"
#include "Analyzer.hpp"
#include "TestHelper.hpp" // hidden file

auto data = R"(
[
    {
        "orderId": 554,
        "creationDate": "2017-03-25T10:35:20Z",
        "orderLines": [
            {"productId": 9872, "name": "Pencil", "quantity": 3, "unitPrice": 3.00}
        ]
    },
    {
        "orderId": 555,
        "creationDate": "2017-03-25T11:24:20Z",
        "orderLines": [
            {"productId": 9872, "name": "Pencil", "quantity": 2, "unitPrice": 3.00},
            {"productId": 1746, "name": "Eraser", "quantity": 1, "unitPrice": 1.00}
        ]
    },
    {
        "orderId": 453,
        "creationDate": "2017-03-27T14:53:12Z",
        "orderLines": [
            {"productId": 5723, "name": "Pen", "quantity": 4, "unitPrice": 4.22},
            {"productId": 9872, "name": "Pencil", "quantity": 3, "unitPrice": 3.12},
            {"productId": 3433, "name": "Erasers Set", "quantity": 1, "unitPrice": 6.15}
        ]
    },
    {
        "orderId": 431,
        "creationDate": "2017-03-20T12:15:02Z",
        "orderLines": [
            {"productId": 5723, "name": "Pen", "quantity": 7, "unitPrice": 4.22},
            {"productId": 3433, "name": "Erasers Set", "quantity": 2, "unitPrice": 6.15}
        ]
    },
    {
        "orderId": 690,
        "creationDate": "2017-03-26T11:14:00Z",
        "orderLines": [
            {"productId": 9872, "name": "Pencil", "quantity": 4, "unitPrice": 3.12},
            {"productId": 4098, "name": "Marker", "quantity": 5, "unitPrice": 4.50}
        ]
    }
]
)"_json;

class OrdersTest : public ::testing::Test {
protected:
    std::vector<Order> input;
    Analyzer analyzer;

    virtual void SetUp() {
        for (auto element : data) {
            std::vector<Item> items;
            for (auto item : element["orderLines"]) {
                Item t {
                    item["productId"].get<int>(),
                    item["name"].get<std::string>(),
                    item["quantity"].get<int>(),
                    item["unitPrice"].get<double>()
                };
                items.push_back(t);
            }

            Order tmp {
                element["orderId"].get<int>(),
                toTime(element["creationDate"].get<std::string>()),
                items
            };
            input.push_back(tmp);
        }
    }

    virtual void TearDown() { }
};

TEST_F(OrdersTest, WeekDay) {
     ASSERT_EQ(WeekDay::Monday,    getDay((time_t)631152000));
     ASSERT_EQ(WeekDay::Tuesday,   getDay((time_t)473385600));
     ASSERT_EQ(WeekDay::Wednesday, getDay((time_t)504921600));
     ASSERT_EQ(WeekDay::Thursday,  getDay((time_t)536457600));
     ASSERT_EQ(WeekDay::Friday,    getDay((time_t)567993600));
     
    //ASSERT_EQ(WeekDay::Friday,    getDay((time_t)568079999));
     ASSERT_EQ(WeekDay::Saturday,  getDay((time_t)757382400));
      ASSERT_EQ(WeekDay::Sunday,    getDay((time_t)441763200));
}

TEST_F(OrdersTest, Init) {
    ASSERT_EQ(5, input.size());
    ASSERT_EQ(4098, input[4].items[1].productId);
}

TEST_F(OrdersTest, ValueForPencils) {
    bool result = testResults(analyzer, 9872, input);
    EXPECT_TRUE(result);
}

TEST_F(OrdersTest, ValueForPen) {
    bool result = testResults(analyzer, 5723, input);
    EXPECT_TRUE(result);
}

TEST_F(OrdersTest, ValueForEraserSet) {
    bool result = testResults(analyzer, 3433, input);
    EXPECT_TRUE(result);
}

TEST_F(OrdersTest, ValueForMarker) {
    bool result = testResults(analyzer, 4098, input);
    EXPECT_TRUE(result);
}
