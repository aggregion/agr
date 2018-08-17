FROM agrio/builder
ARG branch=master
ARG symbol=SYS

RUN git clone -b $branch https://github.com/aggregion/agr.git --recursive \
    && cd agr && echo "$branch:$(git rev-parse HEAD)" > /etc/agrio-version \
    && cmake -H. -B"/opt/agrio" -GNinja -DCMAKE_BUILD_TYPE=Release -DWASM_ROOT=/opt/wasm -DCMAKE_CXX_COMPILER=clang++ \
       -DCMAKE_C_COMPILER=clang -DCMAKE_INSTALL_PREFIX=/opt/agrio  -DSecp256k1_ROOT_DIR=/usr/local -DBUILD_MONGO_DB_PLUGIN=true -DCORE_SYMBOL_NAME=$symbol \
    && cmake --build /opt/agrio --target install \
    && cp /agr/Docker/config.ini / && ln -s /opt/agrio/contracts /contracts && cp /agr/Docker/nodagrd.sh /opt/agrio/bin/nodagrd.sh && ln -s /agr/tutorials /tutorials

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get -y install openssl ca-certificates vim psmisc python3-pip && rm -rf /var/lib/apt/lists/*
RUN pip3 install numpy
ENV AGRIO_ROOT=/opt/agrio
RUN chmod +x /opt/agrio/bin/nodagrd.sh
ENV LD_LIBRARY_PATH /usr/local/lib
VOLUME /opt/agrio/bin/data-dir
ENV PATH /opt/agrio/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
