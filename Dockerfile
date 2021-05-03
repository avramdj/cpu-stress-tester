FROM alpine:latest AS build
WORKDIR /build
RUN apk add --no-cache build-base
COPY . .
RUN make compile

FROM alpine:latest AS final
RUN apk add --no-cache libstdc++
WORKDIR /app
COPY --from=build /build/bin/stress .
ENTRYPOINT ["/app/stress"]
