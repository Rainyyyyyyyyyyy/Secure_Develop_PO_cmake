cmake_minimum_required(VERSION 3.10)
project(MyProgram) # Убедитесь, что имя совпадает с тем, что в add_executable

set(CMAKE_CXX_STANDARD 17)

# Указываем где искать заголовочные файлы Crypto++
# (если вы ставили через MSYS2, пути обычно такие)
# include_directories("C:/msys64/ucrt64/include")
include_directories("#{CMAKE_SOURCE_DIR}/sib_dirs/include")
# Указываем где искать готовые библиотеки (.a файлы)
#link_directories("C:/msys64/ucrt64/lib")
link_directories("${CMAKE_SOURCE_DIR}/sub_dirs/lib")



# Создаем исполняемый файл
add_executable(MyProgram main.cpp) # Или перечислите все ваши .cpp файлы

# Линкуем библиотеку cryptopp к нашей программе
target_link_libraries(MyProgram cryptopp)
# target_link_libraries(MyProgram "C:/msys64/ucrt64/lib/libcryptopp.a")
#target_link_directories(MyProgram "${CMAKE_SOURCE_DIR}/sub_dirs/lib")