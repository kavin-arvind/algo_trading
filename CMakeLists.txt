cmake_minimum_required(VERSION 3.20)

project(AlgoTraderRepeat
    VERSION 1.0
    LANGUAGES CXX
)

# ============================================================
# C++ configuration
# ============================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# ============================================================
# Where executables are placed
# ============================================================

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/bin
)


# ============================================================
# Common library
# ============================================================

add_library(algotrader_common

    # Add common .cpp files here as you create them
    src/common/stock.cpp

)

target_include_directories(algotrader_common
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)


# ============================================================
# Trader library
# ============================================================

add_library(algotrader_trader

    src/trader/trader.cpp
    src/trader/traderFactory.cpp
    src/trader/traderPhase1.cpp
    src/trader/receiver.cpp

)

target_include_directories(algotrader_trader
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(algotrader_trader
    PUBLIC
        algotrader_common
)


# ============================================================
# IML library
# ============================================================

add_library(algotrader_iml

    src/iml/iml.cpp

)

target_include_directories(algotrader_iml
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(algotrader_iml
    PUBLIC
        algotrader_common
)


# ============================================================
# Trader executable
# ============================================================

add_executable(trader

    apps/trader/main.cpp

)

target_link_libraries(trader
    PRIVATE
        algotrader_trader
)


# ============================================================
# IML executable
# ============================================================

add_executable(iml

    apps/iml/main.cpp

)

target_link_libraries(iml
    PRIVATE
        algotrader_iml
)


# ============================================================
# Market executable
# ============================================================

add_executable(market

    apps/market/main.cpp

)

target_link_libraries(market
    PRIVATE
        algotrader_common
)