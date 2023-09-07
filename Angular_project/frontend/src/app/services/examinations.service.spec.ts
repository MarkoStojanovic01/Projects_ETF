import { TestBed } from '@angular/core/testing';

import { ExaminationsService } from './examinations.service';

describe('ExaminationsService', () => {
  let service: ExaminationsService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(ExaminationsService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
