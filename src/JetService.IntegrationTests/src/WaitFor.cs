using System;
using System.Threading;
using NUnit.Framework;

namespace JetService.IntegrationTests
{
  public static class WaitFor
  {
    public static void WaitForAssert(Func<bool> predicate, string message, params object[] argz)
    {
      for(int i = 0; i < 6; i++)
      {
        if (predicate()) return;
        Thread.Sleep(TimeSpan.FromSeconds(1 + 2* i));
      }
      Assert.Fail(string.Format(message, argz));
    }
  }
}