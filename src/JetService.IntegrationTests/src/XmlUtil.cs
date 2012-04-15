using System;
using System.IO;
using System.Xml.Serialization;

namespace JetService.IntegrationTests
{
  public static class XmlUtil
  {
    public static void Serialize<T>(T obj, string file)
    {
      using (var wr = File.CreateText(file))
      {
        Serialize(obj, wr);
      }
    }

    public static void Serialize<T>(T obj, TextWriter wr)
    {
      var f = new XmlSerializerFactory();
      var ser = f.CreateSerializer(typeof(T));
      if (ser == null)
        throw new Exception("Failed to get serializer");
      ser.Serialize(wr, obj);
    }

    public static T SClone<T>(this T obj)
    {
      var f = new XmlSerializerFactory();
      var ser = f.CreateSerializer(typeof(T));
      if (ser == null)
        throw new Exception("Failed to get serializer");
      var ms = new MemoryStream();
      ser.Serialize(ms, obj);
      ms = new MemoryStream(ms.GetBuffer());
      return (T) ser.Deserialize(ms);
    }
  }
}