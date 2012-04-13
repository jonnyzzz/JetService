using System;
using System.IO;
using System.Xml.Serialization;

namespace JetService.IntegrationTests
{
  [XmlRoot("jetservice")]
  public class ServiceSettings
  {
    [XmlElement("name")]
    public string Name { get; set; }

    [XmlElement("displayName")]
    public string DisplayName { get; set; }

    [XmlElement("description")]
    public string Description { get; set; }

    [XmlElement("execution")]
    public ExecutionElement Execution { get; set; }

    public ServiceSettings()
    {
      Execution = new ExecutionElement();
    }


    public override string ToString()
    {
      var sw = new StringWriter();
      Serialize(sw);
      return sw.ToString();
    }

    public void Serialize(string file)
    {
      using(var wr = File.CreateText(file))
      {
        Serialize(wr);
      }
    }

    private void Serialize(TextWriter wr)
    {
      var f = new XmlSerializerFactory();
      var ser = f.CreateSerializer(typeof (ServiceSettings));
      if (ser == null)
        throw new Exception("Failed to get serializer");
      ser.Serialize(wr, this);
    }
  }

  [XmlRoot("execution")]
  public class ExecutionElement
  {
    [XmlElement("workdir")]
    public string WorkDir { get; set; }

    [XmlElement("program")]
    public string Program { get; set; }

    [XmlElement("arguments")]
    public string Arguments { get; set; }

    [XmlElement("termination")]
    public TerminationElement Termination { get; set; }
  }

  [XmlRoot("termination")]
  public class TerminationElement
  {
    [XmlAttribute("timeout")]
    public string Timeout
    {
      get { return TerminateTimoeut == null ? null : TerminateTimoeut.Value.ToString(); }
      set { TerminateTimoeut = value == null ? null : (long?) long.Parse(value); }
    }

    [XmlIgnore]
    public long? TerminateTimoeut { get; set; }
  }

}