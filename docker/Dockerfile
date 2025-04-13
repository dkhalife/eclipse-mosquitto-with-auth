FROM eclipse-mosquitto:openssl

RUN apk add --no-cache libcurl;

COPY ../src/auth-plugin.so /plugins/auth-plugin.so

EXPOSE 8883

ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["/usr/sbin/mosquitto", "-c", "/mosquitto/config/mosquitto.conf"]
