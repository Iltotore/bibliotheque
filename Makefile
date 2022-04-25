
BUILD_DIR=build

create_build_dir:
	mkdir -p ${BUILD_DIR}
	
clean:
	rm -r ${BUILD_DIR}

%.o: %.c %.h create_build_dir
	gcc -c -o ${BUILD_DIR}/$@ $<