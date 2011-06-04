using System;
using System.Windows.Forms;

public class WaitCursor : IDisposable
  {
      Cursor cursorOld_;
      bool disposedValue_ = false; // To detect redundant call 

      public WaitCursor()
      {
          cursorOld_ = Cursor.Current;
          Cursor.Current = Cursors.WaitCursor;
      }

      public void Dispose()
      {
          Dispose(true);
          GC.SuppressFinalize(this);
      }

      protected virtual void Dispose(bool disposing)
      {
          if (!disposedValue_) Cursor.Current = cursorOld_;
          disposedValue_ = true;
      }
  }