using System;

namespace JetService.IntegrationTests
{
  public class ServiceInfo
  {
    public string Name { get; set; }
    public string Status { get; set; }

    public bool IsRunning
    {
      get { return Status == "Running" || Status == "Starting" || Status == "Start Pending"; }
    }

    public bool IsStopped
    {
      get { return Status == "Stopped" || Status == "Disabled"; }
    }

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