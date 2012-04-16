using System;

namespace JetService.IntegrationTests
{
  public class ServiceInfo
  {
    public string Name { get; set; }
    public string Status { get; set; }


    public override string ToString()
    {
      return string.Format("Service{{Name={0}, Status={1}}}", Name, Status);
    }

    public bool IsNamed(string name)
    {
      return Name.Equals(name, StringComparison.CurrentCultureIgnoreCase);
    }
  }
}