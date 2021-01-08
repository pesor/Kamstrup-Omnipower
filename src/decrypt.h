bool decrypt(const VectorView &frame)
{

  if (frame.size() < headersize + footersize + 12 + 18)
  {
    Serial.println("Invalid frame size.");
  }

  memcpy(decryptedFrameBuffer, &frame.front(), frame.size());

  uint8_t system_title[8];
  memcpy(system_title, decryptedFrameBuffer + headersize + 2, 8);

  uint8_t initialization_vector[12];
  memcpy(initialization_vector, system_title, 8);
  memcpy(initialization_vector + 8, decryptedFrameBuffer + headersize + 14, 4);

  uint8_t additional_authenticated_data[17];
  memcpy(additional_authenticated_data, decryptedFrameBuffer + headersize + 13, 1);
  memcpy(additional_authenticated_data + 1, authentication_key, 16);

  uint8_t authentication_tag[12];
  memcpy(authentication_tag, decryptedFrameBuffer + headersize + frame.size() - headersize - footersize - 12, 12);

  uint8_t cipher_text[frame.size() - headersize - footersize - 18 - 12];
  memcpy(cipher_text, decryptedFrameBuffer + headersize + 18, frame.size() - headersize - footersize - 12 - 18);

  uint8_t plaintext[sizeof(cipher_text)];

  mbedtls_gcm_init(&m_ctx);
  int success = mbedtls_gcm_setkey(&m_ctx, MBEDTLS_CIPHER_ID_AES, encryption_key, sizeof(encryption_key) * 8);
  if (0 != success)
  {
    Serial.println("Setkey failed: " + String(success));
    return false;
  }
  success = mbedtls_gcm_auth_decrypt(&m_ctx, sizeof(cipher_text), initialization_vector, sizeof(initialization_vector),
                                     additional_authenticated_data, sizeof(additional_authenticated_data), authentication_tag, sizeof(authentication_tag),
                                     cipher_text, plaintext);
  if (0 != success)
  {
    Serial.println("authdecrypt failed: " + String(success));
    return false;
  }
  mbedtls_gcm_free(&m_ctx);

  //copy replace encrypted data with decrypted for mbusparser library. Checksum not updated. Hopefully not needed
  memcpy(decryptedFrameBuffer + headersize + 18, plaintext, sizeof(plaintext));
  decryptedFrame = VectorView(decryptedFrameBuffer, frame.size());

  return true;
}