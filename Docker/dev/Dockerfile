FROM enumivo/builder as builder
ARG branch=master
ARG symbol=ENU

RUN git clone -b $branch https://github.com/enumivo/enumivo.git --recursive \
    && cd enumivo && echo "$branch:$(git rev-parse HEAD)" > /etc/enumivo-version \
    && cmake -H. -B"/opt/enumivo" -GNinja -DCMAKE_BUILD_TYPE=Release -DWASM_ROOT=/opt/wasm -DCMAKE_CXX_COMPILER=clang++ \
       -DCMAKE_C_COMPILER=clang -DCMAKE_INSTALL_PREFIX=/opt/enumivo -DBUILD_MONGO_DB_PLUGIN=true -DCORE_SYMBOL_NAME=$symbol \
    && cmake --build /opt/enumivo --target install \
    && cp /enumivo/Docker/config.ini / && ln -s /opt/enumivo/contracts /contracts && cp /enumivo/Docker/nodenud.sh /opt/enumivo/bin/nodenud.sh && ln -s /enumivo/tutorials /tutorials

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get -y install openssl ca-certificates vim psmisc python3-pip && rm -rf /var/lib/apt/lists/*
RUN pip3 install numpy
ENV ENUMIVO_ROOT=/opt/enumivo
RUN chmod +x /opt/enumivo/bin/nodenud.sh
ENV LD_LIBRARY_PATH /usr/local/lib
ENV PATH /opt/enumivo/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
