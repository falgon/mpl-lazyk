FROM gcc:7.5.0
SHELL ["/bin/bash", "-c"]
USER root
LABEL maintainer="Roki <falgon53@gmail.com>"
ENV DEBCONF_NOWARNINGS yes
RUN apt-get -qq update \
    && apt-get -qq -y upgrade \
    && apt-get install --no-install-recommends -qq -y \
        make \
    && apt-get -qq -y autoremove \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /proj
RUN wget -q -O boost_1_65_1.tar.gz http://sourceforge.net/projects/boost/files/boost/1.65.1/boost_1_65_1.tar.gz/download \
    && tar xzf boost_1_65_1.tar.gz \
    && cd boost_1_65_1 \
    && ./bootstrap.sh \
    && ./b2 link=static,shared install -d0 -j1 --prefix=/proj --with-test \
    && mv /proj/include/boost /usr/local/include/ \
	&& mv /proj/lib/* /usr/local/lib/ \
	&& cd /proj \
    && rm -r boost_1_65_1 boost_1_65_1.tar.gz 
ENV LD_LIBRARY_PATH /usr/local/lib:$LD_LIBRARY_PATH
