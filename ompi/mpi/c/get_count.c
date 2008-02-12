/*
 * Copyright (c) 2004-2007 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2008 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 */
#include "ompi_config.h"
#include <stdio.h>

#include "ompi/mpi/c/bindings.h"
#include "ompi/datatype/datatype.h"
#include "ompi/include/ompi/memchecker.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILING_DEFINES
#pragma weak MPI_Get_count = PMPI_Get_count
#endif

#if OMPI_PROFILING_DEFINES
#include "ompi/mpi/c/profile/defines.h"
#endif

static const char FUNC_NAME[] = "MPI_Get_count";


int MPI_Get_count(MPI_Status *status, MPI_Datatype datatype, int *count) 
{
   size_t size = 0;
   int rc      = MPI_SUCCESS;

    MEMCHECKER(
        if (status != MPI_STATUSES_IGNORE) {
            /*
             * Before checking the complete status, we need to reset the definedness
             * of the MPI_ERROR-field (single-completion calls wait/test).
             */
            opal_memchecker_base_mem_defined(&status->MPI_ERROR, sizeof(int));
            memchecker_status(status);
            memchecker_datatype(datatype);
        }
    );

    OPAL_CR_TEST_CHECKPOINT_READY();

   if (MPI_PARAM_CHECK) {
      OMPI_ERR_INIT_FINALIZE(FUNC_NAME);
      OMPI_CHECK_DATATYPE_FOR_RECV(rc, datatype, 1);

      OMPI_ERRHANDLER_CHECK(rc, MPI_COMM_WORLD, rc, FUNC_NAME);
   }

   if( ompi_ddt_type_size( datatype, &size ) == MPI_SUCCESS ) {
      if( size == 0 ) {
         *count = 0;
      } else {
         *count = (int)(status->_count / size);
         if( (int)((*count) * size) != status->_count )
            *count = MPI_UNDEFINED;
      }
   }
   return MPI_SUCCESS;
}
