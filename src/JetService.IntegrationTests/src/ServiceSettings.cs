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
      XmlUtil.Serialize(this, file);
    }

    private void Serialize(TextWriter wr)
    {
      XmlUtil.Serialize(this, wr);
    }
  }

  public class ExecutionBase
  {
    [XmlElement("workdir")]
    public string WorkDir { get; set; }

    [XmlElement("program")]
    public string Program { get; set; }

    [XmlElement("arguments")]
    public string Arguments { get; set; }
  }


  [XmlRoot("execution")]
  public class ExecutionElement : ExecutionBase
  {
    [XmlElement("termination")]
    public TerminationElement Termination { get; set; }
  }

  [XmlRoot("termination")]
  public class TerminationElement
  {
    [XmlAttribute("timeout")]
    public string Timeout { get; set; }
    
    [XmlIgnore]
    public long? TerminateTimoeut {
      get
      {
        long x;
        return Timeout == null 
          ? null 
          : long.TryParse(Timeout, out x) 
          ? x 
          : (long?) null;
      }
      set
      {
        Timeout = value == null ? null : value.Value.ToString();
      }
    }

    [XmlElement("execution")]
    public ExecutionBase Execution { get; set; }
  }
}