FROM ubuntu:bionic

# Mount volumes - these are in root to make it easier to type 
# and to avoid confusion between those included in the Mesher directory

VOLUME /output
VOLUME /input

RUN apt-get update \
    && apt-get install -y \
    build-essential \
    cmake \
    libcgal-dev \
    libcgal-qt5-dev \
    libglu1-mesa \
    libglu1-mesa-dev \
    libxi-dev \
    libxmu-dev 

# Copy repo and make dirs
COPY . /Mesher

WORKDIR /Mesher
RUN mkdir build && mkdir output

# remove test from cmake list 
RUN sed -i 's/^\(add_subdirectory(test).*\)/#\1/g' CMakeLists.txt

WORKDIR /Mesher/build
RUN cmake .. && make
WORKDIR /Mesher

#RUN echo "BUILD DONE. CHECKING MESHER RUNS"
#RUN ./bin/mesher -i inputs/input.inr -e inputs/Electrodes.txt -p inputs/params.txt

RUN apt-get clean \
    && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

ENTRYPOINT [ "./bin/mesher" ]
# default run with example inputs inside the container to demonstrate its working
CMD ["-i", "inputs/input.inr", "-e", "inputs/Electrodes.txt", "-p", "inputs/params.txt"]

# To run the Mesher, you need to mount the volumes and then direct the mesher to look into the correct directories e.g.:
# docker run --rm -v ~/Mesher/inputs2:/input -v ~/Mesher/output2:/output  mesher -i /input/input.inr -e /input/Electrodes.txt -p /input/params.txt -d /output/ -o testing

