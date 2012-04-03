using System;

namespace JetService.IntegrationTests
{
  public class Disposable : IDisposable
  {
    private readonly Action myAction;

    public Disposable(Action action)
    {
      myAction = action;
    }

    public void Dispose()
    {
      myAction();
    }
  }
}