#include <iostream>
#include "gtest/gtest.h"
#include "SoundexListener.h"

using namespace testing;

int main(int argc, char** argv) {
	InitGoogleTest(&argc, argv);
    TestEventListeners& listeners =
        UnitTest::GetInstance()->listeners();
    listeners.Append(new SoundexListener);
	RUN_ALL_TESTS();
	std::getchar();
}
