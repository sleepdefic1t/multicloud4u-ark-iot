#include <Arduino.h>
#include <WiFi.h>

#include <arkClient.h>
#include <arkCrypto.h>

#include "crypto/hash.hpp"
#include "networks/devnet.hpp"
#include "transactions/builders/builder.hpp"
#include "utils/hex.hpp"

using namespace Ark::Client;
using namespace Ark::Crypto;
using namespace Ark::Crypto::transactions;

// #define SSID "wifiSSID"
// #define PASSWORD "wifiPassword"
// #define MNEMONIC "this is a top secret mnemonic"
// #define RECIPIENT_ID "DLsMhiUzAVEXBXDTY1NGNZteWz8SDvphfa"
#include "secret.h"


Connection<Api> client("167.114.29.52", 4003);

char chipHash[HASH_64_LEN + 1] = {'0'};

uint64_t getWalletNonce(Connection<Api>& client, const char* address) {
  const auto walletResponse = client.api.wallets.get(address);

  const size_t capacity =
      JSON_OBJECT_SIZE(0) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(6) + 160;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, walletResponse);
  uint64_t nonce = doc["data"]["nonce"];

  return nonce + 1;
}

void idHashToBuffer(char hashBuffer[64]) {
  int idByteLen = 6;
  uint64_t chipId = 123456; //  ESP.getEfuseMac();
  uint8_t* bytArray = *reinterpret_cast<uint8_t(*)[sizeof(uint64_t)]>(&chipId);
  std::reverse(&bytArray[0], &bytArray[idByteLen]);
  const auto shaHash = Hash::sha256(&bytArray[0], idByteLen);
  memmove(hashBuffer,
          BytesToHex(&shaHash[0], &shaHash[0] + HASH_32_LEN).c_str(),
          HASH_64_LEN);
}

Transaction createTransaction() {
  const auto nonce = getWalletNonce(client, RECIPIENT_ID);

  idHashToBuffer(chipHash);

  const auto transaction = builder::Transfer()
                                .nonce(nonce)
                                .amount(1)
                                .expiration(0)
                                .recipientId(RECIPIENT_ID)
                                .vendorField(chipHash)
                                .sign(MNEMONIC)
                                .build();

  Serial.printf("\nNew Transaction: %s", transaction.toJson().c_str());

  return transaction;
}

void sendTransaction() {
  const auto transaction = createTransaction();
  auto txPayload = "{\"transactions\":[" + transaction.toJson() + "]}";

  const auto broadcastResponse = client.api.transactions.send(txPayload);
  Serial.printf("\nbroadcastResponse: %s\n", broadcastResponse.c_str());
}

bool txWasFound() {
  const auto txSearchResponse = client.api.wallets.transactionsReceived(RECIPIENT_ID, "?page=1&limit=3");

  return txSearchResponse.find(chipHash) < txSearchResponse.length();
}

void blink() {
  auto blinkCount = 0;
  do {
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
    ++blinkCount;
  } while (blinkCount < 10);

  esp_deep_sleep_start();
}

void setup() {
  Serial.begin(9600);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  pinMode(13, OUTPUT);

  sendTransaction();
}

void loop() {
  if (txWasFound()) {
    Serial.printf("\nTX was found!");
    blink();
  } else {
    Serial.printf("\nSearching For Tx..");
  }
}
