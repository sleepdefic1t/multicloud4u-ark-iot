# MultiCloud4u ARK IoT Presentation Code

[ARK.io](https://ark.io) IoT Blockchain Presentation Project Code created for [MultiCloud4u](https://multicloud4u.com/).

> [meetup.com/Delhi-Cloud-Computing-AI-and-Blockchain-Meetup-Group/events/271158348/](https://www.meetup.com/Delhi-Cloud-Computing-AI-and-Blockchain-Meetup-Group/events/271158348/)

## Objective

Use ARK to leverage the power of IoT using:
- VSCode
- PlatformIO
- ESP32 Development Board

## ARK C++ SDK Compatibility

- [ARK Cpp-Client >= 1.4.1](https://github.com/ArkEcosystem/cpp-client/releases/tag/1.4.1)
- [ARK Cpp-Crypto >= 1.1.0](https://github.com/ArkEcosystem/cpp-crypto/releases/tag/1.1.0)

## Steps

1) Get the Wallet Nonce.
2) Create the Transaction.
3) Broadcast the Transaction.
4) Search for the Transaction.
5) Blink LED when the Transaction is successfully forged on the ARK Blockchain.

## Example Output:
```shell
New Transaction: {"version":2,"network":30,"typeGroup":1,"type":0,"nonce":"63","senderPublicKey":"0216fa03d378b6ad01325e186ad2cbb9d18976d5b27d0ca74b4f92bb6bf9a6d4d9","fee":"10000000","amount":"1","vendorField":"4571846c2d2282934613629e2bc433422f22c6c4ebb80fbcd8cc35026932a8b9","expiration":0,"recipientId":"DNSrsDUq5injGBdNXPV7v7u1Qy9LZfWEdM","id":"ac6c89bab7cf8c1d54005e698368be7311a0b8a40844cb0a6a768d8b63aa882c","signature":"304502210081edf6eb6de17e1d0893bdf87e49776de039273c6183587fdea7dfa9bb692f5e02204d6968d6381547f3ac6c7b233fd4cbcd25ae4f6f20d8ab78fb9e2b0c23f2fca1"}

broadcastResponse: {"data":{"accept":["ac6c89bab7cf8c1d54005e698368be7311a0b8a40844cb0a6a768d8b63aa882c"],"broadcast":["ac6c89bab7cf8c1d54005e698368be7311a0b8a40844cb0a6a768d8b63aa882c"],"excess":[],"invalid":[]}}

Searching For Tx..
Searching For Tx..
Searching For Tx..
Searching For Tx..
TX was found!
```
