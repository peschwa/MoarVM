/* Operation table for I/O. A given handle type may implement any number of
 * these sections. */
struct MVMIOOps {
    /* The various sections that may be implemented. */
    MVMIOClosable     *closable;
    MVMIOEncodable    *encodable;
    MVMIOSyncReadable *sync_readable;
    MVMIOSyncWritable *sync_writable;
    MVMIOSeekable     *seekable;
    MVMIOBindable     *bindable;
    MVMIOInteractive  *interactive;
    MVMIOLockable     *lockable;

    /* How to mark the handle's data, if needed. */
    void (*gc_mark) (MVMThreadContext *tc, void *data, MVMGCWorklist *worklist);

    /* How to free the handle's data. */
    void (*gc_free) (MVMThreadContext *tc, MVMObject *h, void *data);
};

/* I/O operations on handles that can be closed. */
struct MVMIOClosable {
    void (*close) (MVMThreadContext *tc, MVMOSHandle *h);
};

/* I/O operations on handles that can do encoding to/from MVMString. */
struct MVMIOEncodable {
    void (*set_encoding) (MVMThreadContext *tc, MVMOSHandle *h, MVMint64 encoding);
};

/* I/O operations on handles that can do synchronous reading. */
struct MVMIOSyncReadable {
    void (*set_separator) (MVMThreadContext *tc, MVMOSHandle *h, MVMString *sep);
    MVMString * (*read_line) (MVMThreadContext *tc, MVMOSHandle *h);
    MVMString * (*slurp) (MVMThreadContext *tc, MVMOSHandle *h);
    MVMString * (*read_chars) (MVMThreadContext *tc, MVMOSHandle *h, MVMint64 chars);
    MVMint64 (*read_bytes) (MVMThreadContext *tc, MVMOSHandle *h, char **buf, MVMint64 bytes);
    MVMint64 (*eof) (MVMThreadContext *tc, MVMOSHandle *h);
};

/* I/O operations on handles that can do synchronous writing. */
struct MVMIOSyncWritable {
    MVMint64 (*write_str) (MVMThreadContext *tc, MVMOSHandle *h, MVMString *s, MVMint64 newline);
    MVMint64 (*write_bytes) (MVMThreadContext *tc, MVMOSHandle *h, char *buf, MVMint64 bytes);
    void (*flush) (MVMThreadContext *tc, MVMOSHandle *h);
    void (*truncate) (MVMThreadContext *tc, MVMOSHandle *h, MVMint64 bytes);
};

/* I/O operations on handles that can seek/tell. */
struct MVMIOSeekable {
    void (*seek) (MVMThreadContext *tc, MVMOSHandle *h, MVMint64 offset, MVMint64 whence);
    MVMint64 (*tell) (MVMThreadContext *tc, MVMOSHandle *h);
};

/* I/O operations on handles that can bind. */
struct MVMIOBindable {
    void (*bind) (MVMThreadContext *tc, MVMOSHandle *h, MVMString *host, MVMint64 port);
    MVMObject * (*accept) (MVMThreadContext *tc, MVMOSHandle *h);
};

/* I/O operations on handles that can do interactive readline. */
struct MVMIOInteractive {
    MVMString * (*read_line) (MVMThreadContext *tc, MVMOSHandle *h, MVMString *prompt);
};

/* I/O operations on handles that can lock/unlock. */
struct MVMIOLockable {
    MVMint64 (*lock) (MVMThreadContext *tc, MVMOSHandle *h, MVMint64 flag);
    void (*unlock) (MVMThreadContext *tc, MVMOSHandle *h);
};

void MVM_io_close(MVMThreadContext *tc, MVMObject *oshandle);
void MVM_io_set_encoding(MVMThreadContext *tc, MVMObject *oshandle, MVMString *encoding_name);
void MVM_io_seek(MVMThreadContext *tc, MVMObject *oshandle, MVMint64 offset, MVMint64 flag);
MVMint64 MVM_io_tell(MVMThreadContext *tc, MVMObject *oshandle);
MVMString * MVM_io_readline(MVMThreadContext *tc, MVMObject *oshandle);
MVMString * MVM_io_read_string(MVMThreadContext *tc, MVMObject *oshandle, MVMint64 length);
void MVM_io_read_bytes(MVMThreadContext *tc, MVMObject *oshandle, MVMObject *result, MVMint64 length);
MVMString * MVM_io_slurp(MVMThreadContext *tc, MVMObject *oshandle);
MVMint64 MVM_io_write_string(MVMThreadContext *tc, MVMObject *oshandle, MVMString *str, MVMint8 addnl);
void MVM_io_write_bytes(MVMThreadContext *tc, MVMObject *oshandle, MVMObject *buffer);
MVMint64 MVM_io_eof(MVMThreadContext *tc, MVMObject *oshandle);
MVMint64 MVM_io_lock(MVMThreadContext *tc, MVMObject *oshandle, MVMint64 flag);
void MVM_io_unlock(MVMThreadContext *tc, MVMObject *oshandle);
void MVM_io_flush(MVMThreadContext *tc, MVMObject *oshandle);
void MVM_io_truncate(MVMThreadContext *tc, MVMObject *oshandle, MVMint64 offset);
