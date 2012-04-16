using System;
using System.Windows.Forms;

namespace JetService.IntegrationTests.WinForms
{
  public partial class Form1 : Form
  {
    public Action<Form1> AfterFormIsCreated { get; set; }
    public Form1()
    {
      InitializeComponent();
      
    }

    public WebBrowser Browser { get { return webBrowser1; } }

    protected override void OnCreateControl()
    {
      base.OnCreateControl();
      Invoke((Action)DoAfterCreated);
    }

    private void DoAfterCreated()
    {
      try
      {
        AfterFormIsCreated(this);
      } catch(Exception e)
      {
        Console.Out.WriteLine(e);
      }
    }
  }
}
