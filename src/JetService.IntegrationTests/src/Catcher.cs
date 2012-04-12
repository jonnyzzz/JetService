using System;
using System.Collections.Generic;
using NUnit.Framework;

namespace JetService.IntegrationTests
{
  public class Catcher : IDisposable
  {
    private readonly List<Exception> myErrors = new List<Exception>();
    private readonly List<Action> myDispose = new List<Action>();

    public void Execute(Action a, Action dispose = null)
    {
      Execute(() =>
                {
                  a();
                  return "";
                }, dispose);
    }

    public T Execute<T>(Func<T> a, Action dispose = null)
    {
      if (myErrors.Count > 0) return default(T);

      if (dispose != null)
        myDispose.Add(dispose);

      return ExecuteInternal(a);
    }

    private void ExecuteInternal(Action a)
    {
      ExecuteInternal(() =>
                        {
                          a();
                          return "";
                        });
    }

    private T ExecuteInternal<T>(Func<T> a)
    {
      try
      {
        return a();
      }
      catch (Exception e)
      {
        myErrors.Add(e);
      }
      return default(T);
    }

    public void Dispose()
    {
      myDispose.Reverse();     
      foreach (var action in myDispose)
      {
        ExecuteInternal(action);
      }

      foreach (var exception in myErrors)
      {
        Console.Out.WriteLine("Exception: {0}", exception);
      }
      Assert.IsTrue(myErrors.Count == 0, "Some errors occured");
    }
  }
}