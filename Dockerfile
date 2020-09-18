FROM ubuntu:bionic

# Mount volumes - these are in root to make it easier to type (/in/param.txt rather than /Mesher/inputs/param.txt)
# and to avoid confusion between those included in the Mesher repository 

VOLUME /out
VOLUME /in

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
# default run with example inputs inside the container to demonstrate its working. Note these directories are with respect to the current folder /Mesher/ set by WORKDIR 
CMD ["-i", "inputs/input.inr", "-e", "inputs/Electrodes.txt", "-p", "inputs/params.txt"]

# To run the Mesher, you need to mount the volumes and then direct the mesher to look into the correct directories e.g.:
# docker run --rm -v path/to/inputs:/in -v path/to/outputs:/out  mesher -i /in/input.inr -e /in/Electrodes.txt -p /in/params.txt -d /out/ -o testing

