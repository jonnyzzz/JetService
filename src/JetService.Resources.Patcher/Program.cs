using System;
using System.IO;
using System.Text.RegularExpressions;
using System.Linq;

namespace JetService.Resources.Patcher
{
  class Program
  {
    private static string FindResources(string path)
    {
      while (path != null)
      {
        var file = Path.Combine(path, "src", "JetService", "resourece.rc");
        if (File.Exists(file)) return Path.GetFullPath(file);
        path = Path.GetDirectoryName(path);
      }
      throw new Exception("Failed to find resources file patch");
    }

    static int Main(string[] args)
    {
      string version = Environment.GetEnvironmentVariable("BUILD_NUMBER") ?? args.FirstOrDefault();
      Console.Out.WriteLine("Patching config to version: {0}", version);
      if (version == null) return 1;

      string cVersion = version.Replace('.', ',');
      string qVersion = "\"" + version + "\"";

      var path = new Uri(typeof (Program).Assembly.CodeBase).LocalPath;
      var resource = FindResources(path);

      File.Copy(resource, resource + ".bak", true);
      var text = File.ReadAllText(resource);


      text = Regex.Replace(text, @"^(.*FILEVERSION ).*$", "$1 " + cVersion, RegexOptions.Multiline);
      text = Regex.Replace(text, @"^(.*PRODUCTVERSION ).*$", "$1 " + cVersion, RegexOptions.Multiline);

      text = Regex.Replace(text, @"^(\s+VALUE\s+\""ProductVersion\"").*$", "$1, " + qVersion, RegexOptions.Multiline);
      text = Regex.Replace(text, @"^(\s+VALUE\s+\""FileVersion\"").*$", "$1, " + qVersion, RegexOptions.Multiline);
      text = Regex.Replace(text, @"^(.*IDS_JET_SERVICE_VERSION ).*$", "$1 " + qVersion, RegexOptions.Multiline);


      Console.Out.WriteLine("Patch completed.");
      File.WriteAllText(resource, text);
      return 0;
    }
  }
}
