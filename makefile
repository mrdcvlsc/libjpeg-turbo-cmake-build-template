debug:
	cmake -S . -B build -DCMAKE_INSTALL_PREFIX=install -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DBUILD_SHARED_LIBS=FALSE -DCMAKE_BUILD_TYPE=Debug
	cmake --build build --config Debug

release:
	cmake -S . -B build -DCMAKE_INSTALL_PREFIX=install -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DBUILD_SHARED_LIBS=FALSE -DCMAKE_BUILD_TYPE=Release
	cmake --build build --config Release

serve_tensorboard:
	python3 generate_tensorboard_data.py
	tensorboard --logdir=runs

clean:
	rm -r build turbojpeg *.pt raw_data_runs runs