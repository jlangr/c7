project(chapter7)
cmake_minimum_required(VERSION 2.6)

include_directories(include ~/boost_1_51_0)
add_definitions(-std=c++0x)

set(sources 
   Book.cpp
   Branch.cpp
   BranchAccess.cpp
   BranchService.cpp
   Catalog.cpp
   ClassificationService.cpp
   Endpoint.cpp
   EndpointValidator.cpp
   Holding.cpp
   HoldingService.cpp
   InventoryReport.cpp
   MailDestination.cpp
   MailMessage.cpp
   Patron.cpp
   PatronAccess.cpp
   PatronService.cpp
   Scanner.cpp
   ScannerState.cpp
   ScannerStateCheckin.cpp
   ScannerStateCheckout.cpp
   ScannerStateInventory.cpp
   ScannerStateWaiting.cpp
   StreamLineReader.cpp
   TimestampSource.cpp
   Transport.cpp
   )
add_library(library ${sources})
target_link_libraries(library pthread)
target_link_libraries(library rt)

