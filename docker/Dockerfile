FROM eclipse-mosquitto:openssl

RUN apk add --no-cache libcurl;

EXPOSE 8883

ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["/usr/sbin/mosquitto", "-c", "/mosquitto/config/mosquitto.conf"]
