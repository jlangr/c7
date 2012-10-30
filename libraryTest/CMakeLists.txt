cmake_minimum_required(VERSION 2.6)

include_directories(/home/jlangr/gmock-1.6.0/include /home/jlangr/gmock-1.6.0/gtest/include ~/boost_1_51_0 ../library/include)
link_directories(/home/jlangr/gmock-1.6.0/mybuild /home/jlangr/gmock-1.6.0/gtest/mybuild /home/jlangr/tddcpp/c7/library/build)
add_definitions(-std=c++0x)

set(sources 
   main.cpp 
   BarcodeUtilTest.cpp
   BookTest.cpp
   BranchAccessTest.cpp
   BranchServiceTest.cpp
   BranchTest.cpp
   CatalogTest.cpp
   ClassificationServiceTest.cpp
   HoldingBarcodeTest.cpp
   HoldingServiceTest.cpp
   HoldingTest.cpp
   InventoryReportTest.cpp
   KeyedFilePersistenceTest.cpp
   KeyedMemoryPersistenceTest.cpp
   PatronAccessTest.cpp
   PatronServiceTest.cpp
   PatronTest.cpp
   PersistenceTest.cpp
   ReportMailerTest.cpp
   RomanTest.cpp
   ScannerStateCheckinTest.cpp
   ScannerStateCheckoutTest.cpp
   ScannerStateInventoryTest.cpp
   ScannerStateWaitingTest.cpp
   ScannerTestData.cpp
   StreamLineReaderTest.cpp
   TimestampSourceTest.cpp
   VectorReaderTest.cpp
   )
add_executable(test ${sources})
target_link_libraries(test pthread)
target_link_libraries(test gmock)
target_link_libraries(test gtest)
target_link_libraries(test rt)
target_link_libraries(test library)

