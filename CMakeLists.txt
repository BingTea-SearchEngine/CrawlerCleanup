cmake_minimum_required(VERSION 3.31)

set(THIS CrawlerCleanup)

project(${THIS} CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

include(FetchContent)

FetchContent_Declare(
    HtmlParser
    GIT_REPOSITORY https://github.com/BingTea-SearchEngine/HtmlParser.git
    GIT_TAG main
)
FetchContent_MakeAvailable(HtmlParser)

set(DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(DomainFinder ${DIR}/domain_finder.cpp)
add_executable(DuplicateDetector ${DIR}/duplicate_detector.cpp)
add_executable(LinkRemover ${DIR}/link_remover.cpp)
add_executable(Parser ${DIR}/parser.cpp)
add_executable(FindOffset ${DIR}/findoffset.cpp)
target_link_libraries(Parser PUBLIC HtmlParser)
