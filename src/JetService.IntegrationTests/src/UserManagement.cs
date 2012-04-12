using System;
using System.DirectoryServices;
using System.DirectoryServices.AccountManagement;
using System.Globalization;
using System.IO;
using System.Security.AccessControl;
using System.Security.Principal;
using NUnit.Framework;

namespace JetService.IntegrationTests
{
  public static class UserManagement
  {

    public static void GiveAllPermissions(string fs)
    {
      if (Directory.Exists(fs))
      {
        DirectorySecurity ds = Directory.GetAccessControl(fs);
        IdentityReference rr = new NTAccount("everyone");
        ds.AddAccessRule(new FileSystemAccessRule(rr, FileSystemRights.FullControl, AccessControlType.Allow));
      } 

      if (File.Exists(fs))
      {
        GiveAllPermissions(Path.GetDirectoryName(fs));
      }
    }

    public static bool CheckUserExists(User u)
    {
      return ThreadUtil.ExecuteSTA(() =>
                              {
                                using (var pc = new PrincipalContext(ContextType.Machine))
                                {
                                  UserPrincipal up = UserPrincipal.FindByIdentity(
                                    pc,
                                    IdentityType.SamAccountName, u.UserName);

                                  return (up != null);
                                }
                              });
    }

    public static void WithNewUser(UserGroup group, Action<User> action)
    {
      var rand = (int) (DateTime.Now - new DateTime(2012, 04, 01)).TotalMilliseconds%9999;
      var u = new User
                 {
                   UserName = "jetsvc" + rand,
                   Password = "jeti" + rand + "z"
                 };
      ThreadUtil.ExecuteSTA(() => CreateAdminUser(u, group));
      try
      {
        action(u);
      }
      catch (Exception e)
      {
        Console.Out.WriteLine("Failed to execute action: {0}", e);
        throw;
      }
      finally
      {
        ThreadUtil.ExecuteSTA(() => RemoveUser(u));
      }
    }

    private static void CreateAdminUser(User u, UserGroup g)
    {
      var dirEntry = new DirectoryEntry("WinNT://" + Environment.MachineName);

      DirectoryEntries entries = dirEntry.Children;
      DirectoryEntry newUser = entries.Add(u.UserName, "User");

      newUser.Properties["FullName"].Add("JetService test user: " + u.UserName);
      newUser.Invoke("SetPassword", new object[] { u.Password });
      newUser.Invoke("Put", new object[] { "UserFlags", 0x10000 }); //password never expires      
      newUser.CommitChanges();
      Console.Out.WriteLine("Created user: {0}", u);
      Console.Out.Flush();

      // Remove the if condition along with the else to create user account in "user" group.
      DirectoryEntry grp = dirEntry.Children.Find(g.Name, "group");
      grp.Invoke("Add", new object[] {newUser.Path.ToString(CultureInfo.InvariantCulture)});
      grp.CommitChanges();
      Console.Out.WriteLine("Created user: {0} added to group: {1}", u, g);
      Console.Out.Flush();
    }

    private static void RemoveUser(User u)
    {
      try
      {
        var localDirectory = new DirectoryEntry("WinNT://" + Environment.MachineName);
        DirectoryEntries users = localDirectory.Children;
        DirectoryEntry user = users.Find(u.UserName);
        users.Remove(user);

        Console.Out.WriteLine("User {0} removed", u);
        Console.Out.Flush();
        
      } catch(Exception e)
      {
        Console.Out.WriteLine("Failed to remove user. {0} {1} {2}", u.UserName, e.Message, e);
        Assert.Ignore("Failed to remove user: {0}", u.UserName);
      }
    }
  }

  public class User
  {
    public string UserName { get; set; }
    public string Password { get; set; }

    public override string ToString()
    {
      return String.Format("User{{Username={0}, Password={1}}}", UserName, Password);
    }
  }

  public class UserGroup
  {
    public static readonly UserGroup Admin = new UserGroup("Administrators");
    public static readonly UserGroup Guest = new UserGroup("Guests");
    public static readonly UserGroup PowerUser = new UserGroup("Power Users");
    public static readonly UserGroup User = new UserGroup("Users");

    public string Name { get; private set; }
    private UserGroup(string name)
    {
      Name = name;
    }

    public override string ToString()
    {
      return String.Format("UserGroup(name={0})", Name);
    }
  }
}