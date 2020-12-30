void printHex(const unsigned char *data, const size_t length)
{
  for (int i = 0; i < length; i++)
  {
    Serial.printf("%02X", data[i]);
  }
}

void printHex(const VectorView &frame)
{
  for (int i = 0; i < frame.size(); i++)
  {
    Serial.printf("%02X", frame[i]);
  }
}