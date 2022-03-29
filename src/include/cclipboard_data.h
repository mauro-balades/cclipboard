
#ifndef CCLIPBOARD_COPY_DATA_H
#define CCLIPBOARD_COPY_DATA_H

class copy_data {
    public:
        char* m_data;
        size_t m_length;

        copy_data(const char* data, size_t length) {
            m_data = new char[length + 1];
            m_length = length;
            strcpy(m_data, data);
        }

        ~copy_data() {
            delete[] m_data;
        }
};

#endif /* CCLIPBOARD_COPY_DATA_H */
