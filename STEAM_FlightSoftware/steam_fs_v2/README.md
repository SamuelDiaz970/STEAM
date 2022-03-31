This code is delivered as an example only. Feel free to integrate it in custom applications. Care must be taken to handle errors which could be ignored in these examples.

To cross-compile this application for the q-card, use the Xiphos SDK:

1. Install the SDK, found in the `sdks` directory of the release repository
```
sudo ./ark-glibc-x86_64-xsc-image-steam-cortexa9hf-neon-toolchain-nodistro.0.sh
```

2. Source the SDK
```
source /opt/xiphos/sdk/ark/environment-setup-cortexa9hf-neon-xiphos-linux-gnueabi
```

3. Configure the build directory
```
./waf configure
```

4. Build the application
```
./waf
```

The binary will be in the build directory: `build/steam_demo`. You can transfer it to the q-card using `scp build/steam_demo root@q7-revb-8030:/tmp`
