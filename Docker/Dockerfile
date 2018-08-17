FROM agrio/builder as builder
ARG branch=master
ARG symbol=SYS

RUN git clone -b $branch https://github.com/aggregion/agr.git --recursive \
    && cd agr && echo "$branch:$(git rev-parse HEAD)" > /etc/agrio-version \
    && cmake -H. -B"/tmp/build" -GNinja -DCMAKE_BUILD_TYPE=Release -DWASM_ROOT=/opt/wasm -DCMAKE_CXX_COMPILER=clang++ \
       -DCMAKE_C_COMPILER=clang -DCMAKE_INSTALL_PREFIX=/tmp/build  -DSecp256k1_ROOT_DIR=/usr/local -DBUILD_MONGO_DB_PLUGIN=true -DCORE_SYMBOL_NAME=$symbol \
    && cmake --build /tmp/build --target install && rm /tmp/build/bin/agriocpp


FROM ubuntu:18.04

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get -y install openssl ca-certificates && rm -rf /var/lib/apt/lists/*
COPY --from=builder /usr/local/lib/* /usr/local/lib/
COPY --from=builder /tmp/build/bin /opt/agrio/bin
COPY --from=builder /tmp/build/contracts /contracts
COPY --from=builder /agr/Docker/config.ini /
COPY --from=builder /etc/agrio-version /etc
COPY --from=builder /agr/Docker/nodagrd.sh /opt/agrio/bin/nodagrd.sh
ENV AGRIO_ROOT=/opt/agrio
RUN chmod +x /opt/agrio/bin/nodagrd.sh
ENV LD_LIBRARY_PATH /usr/local/lib
VOLUME /opt/agrio/bin/data-dir
ENV PATH /opt/agrio/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
