EXECUTABLE=app.out

.DEFAULT_GOAL:=$(EXECUTABLE)

SRCS:=$(wildcard *.c)

clean:
	@rm -f *.o
	@rm -f *.h.gch
	@rm -f $(EXECUTABLE)

$(EXECUTABLE): $(SRCS)
	@echo "Building application..."
	@gcc $(SRCS) -o $(EXECUTABLE) -Wall -Werror

run: $(EXECUTABLE)
	@echo "Running application..."
	@./$(EXECUTABLE)
