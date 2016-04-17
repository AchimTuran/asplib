project(InitDeinit)

set(SOURCES main.cpp)

asplib_source_group("${SOURCES}")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} asplib::SpectrumVisProcessor)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME asplib_SpectrumVisProcessorTest_${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "Tests/SpectrumVisProcessor")
