# Configurações de compilação
## usando g++ não precisa do parâmetro -lstdc++
CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=c++2a
LFLAGS = 
DEF = -DDEV_CODE_ENABLED

# Configurações gerais
SOURCE_FOLDER_NAME=src
SAMPLE_FOLDER_NAME=samples
TESTS_FOLDER_NAME=tests
BUILD_FOLDER_NAME=target

#RUN_ARGS=./samples/customers-100.csv --filter "Customer Id,First Name,Last Name,Company,City,Country,Phone 1,Phone 2,Email,Subscription Date"
#RUN_ARGS=./samples/customers-100.csv --filter "Last Name,Company,City,Country,Phone 1,Phone 2,Email,Subscription Date,Website" --table-print
#RUN_ARGS=./samples/customers-100.csv --infer-types
#RUN_ARGS=./samples/books.csv --emmit-sample-csv --infer-types --table-print
#RUN_ARGS=./samples/books-with-error.csv --verbose --analyze
RUN_ARGS=./samples/imdb_top_1000.csv --filter "Released_Year,Certificate,Runtime,Genre,IMDB_Rating,Overview,Meta_score,Director,Star1,Star2,Star3,Star4,No_of_Votes,Gross" --verbose --table-print

build-folder-setup:
	@ mkdir -p $(BUILD_FOLDER_NAME)

copy-sample-data:
	@ cp -rf ./$(SAMPLE_FOLDER_NAME) ./$(BUILD_FOLDER_NAME)

main: build-folder-setup ./$(SOURCE_FOLDER_NAME)/main.cpp  
	$(CC) ./$(SOURCE_FOLDER_NAME)/main.cpp -o $(BUILD_FOLDER_NAME)/main $(CFLAGS) $(LFLAGS) $(DEF)

tests: build-folder-setup ./$(TESTS_FOLDER_NAME)/main.cpp  
	$(CC) ./$(TESTS_FOLDER_NAME)/main.cpp -o $(BUILD_FOLDER_NAME)/tests $(CFLAGS) $(LFLAGS) $(DEF)

run: main copy-sample-data
	@echo "cd ./$(BUILD_FOLDER_NAME)"
	@echo ./main $(RUN_ARGS)
	@echo "================"
	@cd ./$(BUILD_FOLDER_NAME) && ./main $(RUN_ARGS)
	@echo "\n================\n"

run-tests: tests copy-sample-data
	@echo "cd ./$(BUILD_FOLDER_NAME)"
	@echo ./tests
	@echo "================"
	@cd ./$(BUILD_FOLDER_NAME) && ./tests
	@echo "\n================\n"
